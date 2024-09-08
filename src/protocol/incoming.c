#ifndef _WIN32
#include <sys/socket.h>
#endif
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opcodes.h"
#include "../config/anim.h"
#include "../buffer.h"
#include "../entity.h"
#include "../netio.h"
#include "../script.h"
#include "../server.h"
#include "../shop.h"
#include "../trade.h"
#include "../utility.h"
#include "../zone.h"
#include "../persistence/database.h"

#ifdef _WIN32
#include "../platform/win32_compat.h"
#endif

/*
 * TODO: packet logging is incomplete
 * - should not just output slot for inventory
 */

/* roughly one packet per frame at 50fps */
#define MAX_PACKETS_PER_TICK		(32)

#define MAX_PLAYER_HAIR_COLOUR		(9)
#define MAX_PLAYER_SKIN_COLOUR		(4)
#define MAX_PLAYER_CLOTHING_COLOUR	(14)

static void
process_packet(struct player *, uint8_t *, size_t);

static int
process_login(struct player *, uint8_t *, size_t, size_t);

static int
process_login_isaac(struct player *, uint8_t *, size_t, size_t);

int
player_parse_incoming(struct player *p)
{
	uint8_t *b = NULL;
	int counter = 0;

	while (p->inbuf_processed < p->inbuf_len) {
		if (counter >= MAX_PACKETS_PER_TICK) {
			break;
		}
		if ((p->inbuf_len - p->inbuf_processed) < 2) {
			/* header not yet available */
			break;
		}
		int len = p->inbuf[p->inbuf_processed] & 0xff;
		int len2 = p->inbuf[p->inbuf_processed + 1] & 0xff;
		if (len >= 160) {
			/* "large" packets use two bytes for the length */
			len = (len - 160) * 256 + len2;
		}
		if (len == 0) {
			/* not possible through normal usage */
			printf("len == 0\n");
			break;
		}
		b = malloc(len);
		if (b == NULL) {
			break;
		}
		if (len > 1) {
			if (len < 160) {
				if ((p->inbuf_len - p->inbuf_processed - 1) < len) {
					/* insufficient data in inbuf so far... */
					break;
				}
				memcpy(b, p->inbuf + p->inbuf_processed + 2, len - 1);
				p->inbuf_processed += (len + 1);
			} else {
				if ((p->inbuf_len - p->inbuf_processed - 2) < len) {
					break;
				}
				memcpy(b, p->inbuf + p->inbuf_processed + 2, len);
				p->inbuf_processed += (len + 2);
			}
		} else {
			p->inbuf_processed += (len + 1);
		}
		/* for smaller packets len2 is the last byte */
		if (len < 160) {
			b[len - 1] = len2;
		}
		process_packet(p, b, (size_t)len);
		free(b);
		b = NULL;
		counter++;
	}
	if (p->inbuf_processed >= p->inbuf_len) {
		p->inbuf_processed = 0;
		p->inbuf_len = 0;
	}

	return 0;
}

void
process_packet(struct player *p, uint8_t *data, size_t len)
{
	size_t offset = 0;
	uint32_t opcode = data[offset++] & 0xff;

#if 0
	printf("process packet opcode %d len %zu\n", opcode, len);
#endif

	if (p->login_stage == LOGIN_STAGE_ZERO) {
		if (opcode == OP_CLI_REGISTER ||
		    opcode == OP_CLI_LOGIN || opcode == OP_CLI_RECONNECT) {
			p->protocol_rev = 110;
			p->last_packet = p->mob.server->tick_counter;
		} else if (opcode == 32) {
			uint8_t zero[4] = {0};

			p->protocol_rev = 203;
			p->login_stage = LOGIN_STAGE_SESSION;
			p->last_packet = p->mob.server->tick_counter;

			/* extra bytes for session id */
			(void)send(p->sock, &zero, sizeof(zero), 0);
			return;
		} else {
			return;
		}
	}

	if (p->protocol_rev == 203) {
		if (p->isaac_ready) {
			opcode = (opcode - isaac_next(&p->isaac_in)) & 0xff;
		}
		opcode = opcodes_in_203[opcode];
	}

	p->last_packet = p->mob.server->tick_counter;

	switch (opcode) {
	case OP_CLI_REGISTER:
		{
			if (p->login_stage == LOGIN_STAGE_GOT_LOGIN) {
				return;
			}

			switch (p->protocol_rev) {
			case 110:
				if (process_login(p, data, offset, len) == -1) {
					p->logout_confirmed = true;
					return;
				}
				break;
			case 203:
				if (process_login_isaac(p,
					data, offset, len) == -1) {
					p->logout_confirmed = true;
					return;
				}
				break;
			}

			p->login_stage = LOGIN_STAGE_GOT_LOGIN;

			/*
			 * we never login directly from the registration, it
			 * sends another login packet after account is successfuly
			 * created
			 */
			p->logout_confirmed = true;

			int res = database_new_player(&p->mob.server->database, p);

			memset(p->password, 0, sizeof(p->password));

			if (res == -1) {
				net_login_response(p, RESP_INVALID);
				return;
			}

			net_login_response(p, RESP_REGISTER_OK);
		}
		break;
	case OP_CLI_LOGIN:
	case OP_CLI_RECONNECT:
		{
			if (p->login_stage == LOGIN_STAGE_GOT_LOGIN) {
				return;
			}

			switch (p->protocol_rev) {
			case 110:
				if (process_login(p, data, offset, len) == -1) {
					p->logout_confirmed = true;
					return;
				}
				break;
			case 203:
				if (process_login_isaac(p,
				    data, offset, len) == -1) {
					p->logout_confirmed = true;
					return;
				}
				break;
			}

			p->login_stage = LOGIN_STAGE_GOT_LOGIN;

			int res = player_load(p);

			memset(p->password, 0, sizeof(p->password));

			if (res == -1) {
				p->logout_confirmed = true;
				net_login_response(p, RESP_RATE_LIMIT);
				return;
			}

			if (time(NULL) < p->ban_end_date) {
				p->logout_confirmed = true;
				net_login_response(p, RESP_TEMP_DISABLED);
				return;
			}

			if (res > 0) {
				p->logout_confirmed = true;
				net_login_response(p, res);
				return;
			}

			player_send_privacy_settings(p);

			if (p->login_date == 0) {
				p->mob.x = p->mob.server->start_tile_x;
				p->mob.y = p->mob.server->start_tile_y;

				player_send_design_ui(p);
			} else {
				player_recalculate_equip(p);
				player_send_welcome(p);
			}

			p->login_date = (uint64_t)time(NULL);

			player_send_client_settings(p);
			player_send_quests(p);
			player_send_init_friends(p);
			player_send_init_ignore(p);

			server_register_login(p->name);
			net_login_response(p, RESP_LOGIN_OK);
		}
		break;
	case OP_CLI_LOGOUT:
		{
			packet_log(p, "OP_CLI_LOGOUT\n");
			player_attempt_logout(p, true);
		}
		break;
	case OP_CLI_ADMIN_COMMAND:
		{
			char cmd[128];

			len--;
			if (len > (sizeof(cmd) - 1)) {
				len = (sizeof(cmd) - 1);
			}
			memcpy(cmd, data + offset, len);
			cmd[len] = '\0';

			packet_log(p, "OP_CLI_ADMIN_COMMAND %s\n", cmd);
			player_parse_admin_command(p, cmd);
		}
		break;
	case OP_CLI_PUBLIC_CHAT:
		{
			char mes[MAX_CHAT_LEN];

			size_t msglen = len - 1;
			if (msglen > MAX_CHAT_LEN) {
				msglen = MAX_CHAT_LEN;
			}
			p->chat_type = CHAT_TYPE_PUBLIC;
			if (p->protocol_rev <= 163) {
				memcpy(p->mob.chat_enc, data + offset, msglen);
				p->mob.chat_len = msglen;
				decode_chat_legacy(p->mob.server->words,
				    p->mob.server->num_words,
				    data + 1, msglen, mes, MAX_CHAT_LEN);
				packet_log(p, "OP_CLI_PUBLIC_CHAT %s\n", mes);
				p->mob.chat_compressed_len = chat_compress(mes,
				    p->mob.chat_compressed);
			} else {
				memcpy(p->mob.chat_compressed,
				    data + offset, msglen);
				p->mob.chat_compressed_len = msglen;
				chat_decompress(data, offset, msglen, mes);
				packet_log(p, "OP_CLI_PUBLIC_CHAT %s\n", mes);
				p->mob.chat_len = strlen(mes);
				encode_chat_legacy(mes,
				    (uint8_t *)p->mob.chat_enc, p->mob.chat_len);
			}
		}
		break;
	case OP_CLI_PING:
		{
		}
		break;
	case OP_CLI_ADD_FRIEND:
		{
			int64_t target;
			char name[32];

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			mod37_namedec(target, name);
			packet_log(p, "OP_CLI_ADD_FRIEND %s\n", name);
			if (!player_has_friend(p, target) &&
					!player_has_ignore(p, target)) {
				player_add_friend(p, target);
			}
		}
		break;
	case OP_CLI_REMOVE_FRIEND:
		{
			int64_t target;
			char name[32];

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			mod37_namedec(target, name);
			packet_log(p, "OP_CLI_REMOVE_FRIEND %s\n", name);
			player_remove_friend(p, target);
		}
		break;
	case OP_CLI_PRIVATE_MESSAGE:
		{
			int64_t target;
			char mes[MAX_CHAT_LEN];
			char name[32];

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			if (len <= 9) {
				return;
			}
			len -= 9;
			if (p->protocol_rev <= 163) {
				decode_chat_legacy(p->mob.server->words,
				    p->mob.server->num_words,
				    data + offset, len, mes, MAX_CHAT_LEN);
			} else {
				chat_decompress(data, offset, len, mes);
			}
			mod37_namedec(target, name);
			packet_log(p, "OP_CLI_PRIVATE_MESSAGE %s %s\n",
				    name, mes);
			server_send_pm(p, target, mes);
		}
		break;
	case OP_CLI_ADD_IGNORE:
		{
			int64_t target;
			char name[32];

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			mod37_namedec(target, name);
			packet_log(p, "OP_CLI_ADD_IGNORE %s\n", name);
			if (!player_has_friend(p, target) &&
					!player_has_ignore(p, target)) {
				player_add_ignore(p, target);
			}
		}
		break;
	case OP_CLI_REMOVE_IGNORE:
		{
			int64_t target;
			char name[32];

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			mod37_namedec(target, name);
			packet_log(p, "OP_CLI_REMOVE_IGNORE %s\n", name);
			player_remove_ignore(p, target);
		}
		break;
	case OP_CLI_PRIVACY_SETTINGS:
		{
			uint8_t hide_online = false;
			uint8_t block_public = false;
			uint8_t block_private = false;
			uint8_t block_trade = false;
			uint8_t block_duel = false;
			bool unhide = false;

			packet_log(p, "OP_CLI_PRIVACY_SETTINGS\n");

			if (p->protocol_rev < 115) {
				if (buf_getu8(data, offset++, len,
				    &hide_online) == -1) {
					return;
				}
			}
			if (buf_getu8(data, offset++, len, &block_public) == -1) {
				return;
			}
			if (buf_getu8(data, offset++, len, &block_private) == -1) {
				return;
			}
			if (buf_getu8(data, offset++, len, &block_trade) == -1) {
				return;
			}
			if (buf_getu8(data, offset++, len, &block_duel) == -1) {
				return;
			}
			if (!p->block_private && block_private) {
				server_register_hide_status(p);
			} else if (p->block_private && !block_private) {
				unhide = true;
			}
			p->block_public = block_public;
			p->block_private = block_private || hide_online;
			p->block_trade = block_trade;
			p->block_duel = block_duel;
			if (unhide) {
				server_register_unhide_status(p);
			}
		}
		break;
	case OP_CLI_TRADE_CONFIRM:
		packet_log(p, "OP_CLI_TRADE_CONFIRM\n");
		player_trade_confirm(p);
		break;
	case OP_CLI_BANK_DEPOSIT:
		{
			uint16_t id, amount;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;

			if (buf_getu16(data, offset, len, &amount) == -1) {
				return;
			}
			offset += 2;

			packet_log(p, "OP_CLI_BANK_DEPOSIT %d %d\n",
			    id, amount);
			player_deposit(p, id, amount);
		}
		break;
	case OP_CLI_BANK_WITHDRAW:
		{
			uint16_t id, amount;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;

			if (buf_getu16(data, offset, len, &amount) == -1) {
				return;
			}
			offset += 2;

			packet_log(p, "OP_CLI_BANK_WITHDRAW %d %d\n",
			    id, amount);
			player_withdraw(p, id, amount);
		}
		break;
	case OP_CLI_BANK_CLOSE:
		p->ui_bank_open = false;
		packet_log(p, "OP_CLI_BANK_CLOSE\n");
		break;
	case OP_CLI_PRAYER_OFF:
		{
			uint8_t id;

			if (buf_getu8(data, offset++, len, &id) == -1) {
				return;
			}
			packet_log(p, "OP_CLI_PRAYER_OFF %d\n", id);
			if (id < MAX_PRAYERS) {
				player_prayer_disable(p, id);
			}
		}
		break;
	case OP_CLI_PRAYER_ON:
		{
			uint8_t id;

			if (buf_getu8(data, offset++, len, &id) == -1) {
				return;
			}
			packet_log(p, "OP_CLI_PRAYER_ON %d\n", id);
			if (id < MAX_PRAYERS) {
				player_prayer_enable(p, id);
			}
		}
		break;
	case OP_CLI_SAVE_SETTING:
		{
			uint8_t id, value;

			if (buf_getu8(data, offset++, len, &id) == -1) {
				return;
			}
			if (buf_getu8(data, offset++, len, &value) == -1) {
				return;
			}
			if (id < MAX_CLIENT_SETTINGS) {
				p->client_settings[id] = value;
			}
		}
		break;
	case OP_CLI_FOLLOW_PLAYER:
		{
			struct player *p2;
			char name[32], message[64];
			uint16_t target;

			if (buf_getu16(data, offset, len, &target) == -1) {
				return;
			}
			offset += 2;
			if (target >= MAXPLAYERS) {
				return;
			}
			p2 = p->mob.server->players[target];
			if (p2 == NULL) {
				return;
			}
			mod37_namedec(p2->name, name);
			(void)snprintf(message, sizeof(message),
					"Following %s", name);
			player_send_message(p, message);
			p->mob.following_player = (int16_t)target;
		}
		break;
	case OP_CLI_ITEM_CAST:
		{
			uint16_t x, y;
			uint16_t target_id, spell_id;
			struct spell_config *spell;
			struct ground_item *item;

			if (buf_getu16(data, offset, len, &x) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &y) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &target_id) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &spell_id) == -1) {
				return;
			}
			offset += 2;
			spell = server_spell_config_by_id(spell_id);
			item = server_find_ground_item(p, x, y, target_id);
			if (spell != NULL && item != NULL &&
			    spell->type == SPELL_CAST_ON_ITEM) {
				p->action = ACTION_ITEM_CAST;
				p->spell = spell;
				memcpy(&p->action_item, item,
					sizeof(struct ground_item));
			}
		}
		break;
	case OP_CLI_NPC_CAST:
		{
			uint16_t target_id, spell_id;
			struct npc *npc;
			struct spell_config *spell;

			if (buf_getu16(data, offset, len, &target_id) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &spell_id) == -1) {
				return;
			}
			offset += 2;
			if (target_id >= MAXNPCS) {
				return;
			}
			spell = server_spell_config_by_id(spell_id);
			npc = p->mob.server->npcs[target_id];
			if (npc != NULL && spell != NULL &&
			    spell->type == SPELL_CAST_ON_MOB) {
				p->action = ACTION_NPC_CAST;
				p->spell = spell;
				p->action_npc = npc->mob.id;
			}
		}
		break;
	case OP_CLI_PLAYER_CAST:
		{
			uint16_t target_id, spell_id;
			struct player *target;
			struct spell_config *spell;

			if (buf_getu16(data, offset, len, &target_id) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &spell_id) == -1) {
				return;
			}
			offset += 2;
			if (target_id >= MAXPLAYERS) {
				return;
			}
			target = p->mob.server->players[target_id];
			spell = server_spell_config_by_id(spell_id);
			if (target != NULL && spell != NULL &&
			    (spell->type == SPELL_CAST_ON_MOB ||
			    spell->type == SPELL_CAST_ON_PLAYER)) {
				p->action = ACTION_PLAYER_CAST;
				p->spell = spell;
				p->action_player = target->mob.id;
			}
		}
		break;
	case OP_CLI_SELF_CAST:
		{
			uint16_t spell_id;
			struct spell_config *spell;

			if (buf_getu16(data, offset, len, &spell_id) == -1) {
				return;
			}
			offset += 2;
			spell = server_spell_config_by_id(spell_id);
			if (spell != NULL && spell->type == SPELL_CAST_ON_SELF) {
				p->action = ACTION_SELF_CAST;
				p->spell = spell;
			}
		}
		break;
	case OP_CLI_ATTACK_PLAYER:
		{
			uint16_t id;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;
			if (id < MAXPLAYERS) {
				p->action = ACTION_PLAYER_ATTACK;
				p->action_player = id;
			}
		}
		break;
	case OP_CLI_COMBAT_STYLE:
		{
			uint8_t style;

			if (buf_getu8(data, offset, len, &style) == -1) {
				return;
			}
			packet_log(p, "OP_CLI_COMBAT_STYLE %d\n", style);
			if (style <= COMBAT_STYLE_DEFENSIVE) {
				p->combat_style = style;
			}
		}
		break;
	case OP_CLI_NPC_ATTACK:
		{
			uint16_t id;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;
			if (id < MAXNPCS) {
				packet_log(p, "OP_CLI_NPC_ATTACK %d\n", id);
				p->action = ACTION_NPC_ATTACK;
				p->action_npc = id;
			}
		}
		break;
	case OP_CLI_NPC_USEWITH:
		{
			uint16_t id, slot;
			struct npc *npc;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			offset += 2;
			if (id >= MAXNPCS) {
				return;
			}
			npc = p->mob.server->npcs[id];
			if (npc != NULL) {
				packet_log(p, "OP_CLI_NPC_USEWITH %s %d\n",
				    npc->config->names[0], slot);
				p->action = ACTION_NPC_USEWITH;
				p->action_npc = npc->mob.id;
				p->action_slot = slot;
			}
		}
		break;
	case OP_CLI_NPC_TALK:
		{
			uint16_t id;
			struct npc *npc;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			if (id >= MAXNPCS) {
				return;
			}
			npc = p->mob.server->npcs[id];
			if (npc != NULL) {
				packet_log(p, "OP_CLI_NPC_TALK %s\n",
				    npc->config->names[0]);
				p->action = ACTION_NPC_TALK;
				p->action_npc = npc->mob.id;
			}
		}
		break;
	case OP_CLI_INV_OP1:
		{
			uint16_t slot;

			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			packet_log(p, "OP_CLI_INV_OP1 %d\n", slot);
			if (slot < p->inv_count) {
				p->action = ACTION_INV_OP1;
				p->action_slot = slot;
			}
		}
		break;
	case OP_CLI_INV_WEAR:
		{
			uint16_t slot;

			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			offset += 2;
			packet_log(p, "OP_CLI_INV_WEAR %d\n", slot);
			player_wear(p, slot);
		}
		break;
	case OP_CLI_INV_UNWEAR:
		{
			uint16_t slot;

			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			offset += 2;
			packet_log(p, "OP_CLI_INV_UNWEAR %d\n", slot);
			player_unwear(p, slot);
		}
		break;
	case OP_CLI_INV_DROP:
		{
			uint16_t slot;

			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			offset += 2;
			packet_log(p, "OP_CLI_INV_DROP %d\n", slot);
			p->action = ACTION_INV_DROP;
			p->action_slot = slot;
		}
		break;
	case OP_CLI_ITEM_USEWITH:
		{
			uint16_t x, y, id, slot;
			struct ground_item *item;

			if (buf_getu16(data, offset, len, &x) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &y) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			offset += 2;
			packet_log(p, "OP_CLI_ITEM_USEWITH %d %d %d %d\n",
			    x, y, id, slot);
			item = server_find_ground_item(p, x, y, id);
			if (item != NULL) {
				p->action = ACTION_ITEM_USEWITH;
				p->action_slot = slot;
				memcpy(&p->action_item, item,
					sizeof(struct ground_item));
			}
		}
		break;
	case OP_CLI_ITEM_TAKE:
		{
			uint16_t x, y, id;
			struct ground_item *item;

			if (buf_getu16(data, offset, len, &x) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &y) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;
			item = server_find_ground_item(p, x, y, id);
			if (item != NULL) {
				p->action = ACTION_ITEM_TAKE;
				memcpy(&p->action_item, item,
					sizeof(struct ground_item));
			}
		}
		break;
	case OP_CLI_WALK_TILE:
	case OP_CLI_WALK_ENTITY:
		{
			size_t steps;
			int start_x, start_y;

			if (len < 5 || p->script_active) {
				return;
			}
			if (p->mob.in_combat) {
				if (opcode == OP_CLI_WALK_ENTITY) {
					return;
				} else if (player_retreat(p) == -1) {
					return;
				}
			}
			steps = (len - 5) / 2;
			if (buf_getu16(data, offset, len,
			    &p->mob.walk_queue_x[0]) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len,
			    &p->mob.walk_queue_y[0]) == -1) {
				return;
			}
			offset += 2;
			start_x = p->mob.walk_queue_x[0];
			start_y = p->mob.walk_queue_y[0];
			if (steps > (WALK_QUEUE_LEN - 1)) {
				steps = WALK_QUEUE_LEN - 1;
			}
			for (size_t i = 0; i < steps; ++i) {
				uint8_t off_x, off_y;
				int new_x, new_y;

				if (buf_getu8(data, offset++, len, &off_x) == -1) {
					return;
				}
				if (buf_getu8(data, offset++, len, &off_y) == -1) {
					return;
				}
				new_x = start_x + (int8_t)off_x;
				new_y = start_y + (int8_t)off_y;
				p->mob.walk_queue_x[i + 1] = (uint16_t)new_x;
				p->mob.walk_queue_y[i + 1] = (uint16_t)new_y;
			}
			player_close_ui(p);
			p->mob.following_player = -1;
			p->mob.following_npc = -1;
			if (opcode != OP_CLI_WALK_ENTITY) {
				p->mob.target_npc = -1;
				p->mob.target_player = -1;
				player_clear_actions(p);
			}
			/*
			 * can't perform actions on locs while a step is
			 * pending so don't add steps we've reached
			 */
			if (p->mob.walk_queue_x[steps] != p->mob.x ||
			    p->mob.walk_queue_y[steps] != p->mob.y) {
				if (opcode == OP_CLI_WALK_ENTITY) {
					p->mob.action_walk = true;
					packet_log(p,
					    "OP_CLI_WALK_ENTITY %d %d\n",
					    p->mob.walk_queue_x[steps],
					    p->mob.walk_queue_y[steps]);
				} else {
					p->mob.action_walk = false;
					packet_log(p,
					    "OP_CLI_WALK_TILE %d %d\n",
					    p->mob.walk_queue_x[steps],
					    p->mob.walk_queue_y[steps]);
				}
				p->mob.walk_queue_len = steps + 1;
				p->mob.walk_queue_pos = 0;
			}
		}
		break;
	case OP_CLI_SHOP_SELL:
		{
			uint16_t id;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;
			packet_log(p, "OP_CLI_SHOP_SELL %d\n", id);
			/*
			 * the client also sends the price but we don't trust it
			 * and ignore it
			 */
			if (p->shop != NULL) {
				shop_sell(p->shop, p, id);
			}
		}
		break;
	case OP_CLI_SHOP_BUY:
		{
			uint16_t id;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;
			packet_log(p, "OP_CLI_SHOP_BUY %d\n", id);
			/*
			 * the client also sends the price but we don't trust it
			 * and ignore it
			 */
			if (p->shop != NULL) {
				shop_buy(p->shop, p, id);
			}
		}
		break;
	case OP_CLI_SHOP_CLOSE:
		{
			p->shop = NULL;
			packet_log(p, "OP_CLI_SHOP_CLOSE\n");
			player_close_ui(p);
		}
		break;
	case OP_CLI_INV_CAST:
		{
			uint16_t spell_id, slot;
			struct spell_config *spell;

			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &spell_id) == -1) {
				return;
			}
			offset += 2;
			packet_log(p,
			    "OP_CLI_INV_CAST %d %d\n", slot, spell_id);
			spell = server_spell_config_by_id(spell_id);
			if (spell != NULL) {
				p->action = ACTION_INV_CAST;
				p->spell = spell;
				p->action_slot = slot;
			}
		}
		break;
	case OP_CLI_TRADE_ACCEPT:
		packet_log(p, "OP_CLI_TRADE_ACCEPT\n");
		player_trade_accept(p);
		break;
	case OP_CLI_TRADE_DECLINE:
		packet_log(p, "OP_CLI_TRADE_DECLINE\n");
		player_trade_decline(p);
		break;
	case OP_CLI_TRADE_UPDATE:
		{
			uint8_t count;
			uint16_t id;
			uint32_t amount;
			struct player *partner = NULL;

			if (p->trading_player == -1 || !p->ui_trade_open) {
				return;
			}
			partner = p->mob.server->players[p->trading_player];
			if (partner == NULL) {
				return;
			}

			p->offer_count = 0;
			if (buf_getu8(data, offset++, len, &count) == -1) {
				return;
			}
			packet_log(p, "OP_CLI_TRADE_UPDATE init\n");
			for (int i = 0; i < count; ++i) {
				if (buf_getu16(data, offset, len, &id) == -1) {
					return;
				}
				offset += 2;
				if (buf_getu32(data, offset, len, &amount) == -1) {
					return;
				}
				offset += 4;
				packet_log(p, "OP_CLI_TRADE_UPDATE %d %d\n", id, amount);
				player_trade_offer(p, id, amount);
			}
			partner->partner_offer_changed = true;
		}
		break;
	case OP_CLI_TRADE_PLAYER:
		{
			uint16_t id;
			struct player *target;
			char name[64];

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}

			if (id >= p->mob.server->max_player_id) {
				return;
			}
			target = p->mob.server->players[id];
			if (target != NULL) {
				mod37_namedec(target->name, name);
				packet_log(p, "OP_CLI_TRADE_PLAYER %s\n", name);
				player_trade_request(p, target);
			}
		}
		break;
	case OP_CLI_ACCEPT_DESIGN:
		{
			packet_log(p, "OP_CLI_ACCEPT_DESIGN\n");

			uint8_t old_class = p->rpg_class;

			if (!p->ui_design_open) {
				return;
			}
			(void)buf_getu8(data, offset++, len, &p->gender);
			(void)buf_getu8(data, offset++, len,
			    &p->sprites_base[ANIM_SLOT_HEAD]);
			if (p->sprites_base[ANIM_SLOT_HEAD] != ANIM_HEAD1 &&
			    p->sprites_base[ANIM_SLOT_HEAD] != ANIM_HEAD2 &&
			    p->sprites_base[ANIM_SLOT_HEAD] != ANIM_HEAD3 &&
			    p->sprites_base[ANIM_SLOT_HEAD] != ANIM_HEAD4 &&
			    p->sprites_base[ANIM_SLOT_HEAD] != ANIM_FHEAD1) {
				p->sprites_base[ANIM_SLOT_HEAD] = ANIM_HEAD1;
			}
			p->sprites_base[ANIM_SLOT_HEAD]++;

			(void)buf_getu8(data, offset++, len,
			    &p->sprites_base[ANIM_SLOT_BODY]);
			if (p->sprites_base[ANIM_SLOT_BODY] != ANIM_BODY1 &&
			    p->sprites_base[ANIM_SLOT_BODY] != ANIM_FBODY1) {
				p->sprites_base[ANIM_SLOT_BODY] = ANIM_BODY1;
			}
			p->sprites_base[ANIM_SLOT_BODY]++;

			(void)buf_getu8(data, offset++, len,
				&p->sprites_base[ANIM_SLOT_LEGS]);
			/* authentically only one choice of leg */
			if (p->sprites_base[ANIM_SLOT_LEGS] != ANIM_LEGS1) {
				p->sprites_base[ANIM_SLOT_LEGS] = ANIM_LEGS1;
			}
			p->sprites_base[ANIM_SLOT_LEGS]++;

			(void)buf_getu8(data, offset++, len, &p->hair_colour);
			if (p->hair_colour > MAX_PLAYER_HAIR_COLOUR) {
				p->hair_colour= 0;
			}

			(void)buf_getu8(data, offset++, len, &p->top_colour);
			if (p->top_colour > MAX_PLAYER_CLOTHING_COLOUR) {
				p->top_colour= 0;
			}

			(void)buf_getu8(data, offset++, len, &p->leg_colour);
			if (p->leg_colour > MAX_PLAYER_CLOTHING_COLOUR) {
				p->leg_colour= 0;
			}

			(void)buf_getu8(data, offset++, len, &p->skin_colour);
			if (p->skin_colour > MAX_PLAYER_SKIN_COLOUR) {
				p->skin_colour = 0;
			}

			if (p->protocol_rev < 154) {
				(void)buf_getu8(data, offset++, len, &p->rpg_class);
				if (old_class == UINT8_MAX) {
					player_init_class(p);
				}
			} else {
				p->rpg_class = CLASS_ADVENTURER;
				if (old_class == UINT8_MAX) {
					player_init_adventurer(p);
				}
			}

			player_recalculate_equip(p);
			p->appearance_changed = true;
			p->ui_design_open = false;
			p->script_active = false;

			player_send_welcome(p);
		}
		break;
	case OP_CLI_ANSWER_MULTI:
		{
			uint8_t option;

			if (!p->ui_multi_open) {
				return;
			}
			if (buf_getu8(data, offset++, len, &option) == -1) {
				return;
			}
			packet_log(p, "OP_CLI_ANSWER_MULTI %d\n", option);
			p->ui_multi_open = false;
			script_multi_answer(p->mob.server->lua, p, option);
		}
		break;
	case OP_CLI_BOUND_OP1:
	case OP_CLI_BOUND_OP2:
		{
			struct bound *bound;
			uint16_t x, y;
			uint8_t dir;

			if (buf_getu16(data, offset, len, &x) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &y) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu8(data, offset++, len, &dir) == -1) {
				return;
			}
			bound = server_find_bound(x, y, dir);
			if (bound != NULL) {
				if (opcode == OP_CLI_BOUND_OP1) {
					packet_log(p,
					    "OP_CLI_BOUND_OP1 %d %d %d\n",
					    x, y, dir);
					p->action = ACTION_BOUND_OP1;
				} else {
					packet_log(p,
					    "OP_CLI_BOUND_OP2 %d %d %d\n",
					    x, y, dir);
					p->action = ACTION_BOUND_OP2;
				}
				memcpy(&p->action_bound, bound,
				    sizeof(struct bound));
			}
		}
		break;
	case OP_CLI_BOUND_USEWITH:
		{
			struct bound *bound;
			uint16_t x, y, slot;
			uint8_t dir;

			if (buf_getu16(data, offset, len, &x) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &y) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu8(data, offset++, len, &dir) == -1) {
				return;
			}
			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			offset += 2;
			packet_log(p,
			    "OP_CLI_BOUND_USEWITH %d %d %d %d\n",
			    x, y, dir, slot);
			bound = server_find_bound(x, y, dir);
			if (bound != NULL) {
				p->action = ACTION_BOUND_USEWITH;
				p->action_slot = slot;
				memcpy(&p->action_bound, bound,
				    sizeof(struct bound));
			}
		}
		break;
	case OP_CLI_INV_USEWITH:
		{
			uint16_t slot1, slot2;

			if (buf_getu16(data, offset, len, &slot1) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &slot2) == -1) {
				return;
			}
			offset += 2;
			packet_log(p,
			    "OP_CLI_INV_USEWITH %d %d\n", slot1, slot2);
			p->action = ACTION_INV_USEWITH;
			p->action_slot = slot1;
			p->action_slot2 = slot2;
		}
		break;
	case OP_CLI_LOC_USEWITH:
		{
			uint16_t x, y, slot;
			struct loc *loc;

			if (buf_getu16(data, offset, len, &x) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &y) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &slot) == -1) {
				return;
			}
			offset += 2;
			packet_log(p,
			    "OP_CLI_LOC_USEWITH %d %d %d\n", x, y, slot);
			loc = server_find_loc(x, y);
			if (loc != NULL) {
				p->action = ACTION_LOC_USEWITH;
				p->action_slot = slot;
				memcpy(&p->action_loc, loc, sizeof(struct loc));
			}
		}
		break;
	case OP_CLI_LOC_OP1:
	case OP_CLI_LOC_OP2:
		{
			uint16_t x, y;
			struct loc *loc;

			if (buf_getu16(data, offset, len, &x) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &y) == -1) {
				return;
			}
			offset += 2;
			loc = server_find_loc(x, y);
			if (loc != NULL) {
				if (opcode == OP_CLI_LOC_OP1) {
					packet_log(p,
					    "OP_CLI_LOC_OP1 %d %d\n", x, y);
					p->action = ACTION_LOC_OP1;
				} else {
					packet_log(p,
					    "OP_CLI_LOC_OP2 %d %d\n", x, y);
					p->action = ACTION_LOC_OP2;
				}
				memcpy(&p->action_loc, loc, sizeof(struct loc));
			}
		}
		break;
	}
}

static int
process_login(struct player *p, uint8_t *data, size_t offset, size_t len)
{
	char namestr[64];
	char password[32];
	uint16_t ver;
	int64_t name;

	if (p->name != -1) {
		return -1;
	}
	if (buf_getu16(data, offset, len, &ver) == -1) {
		return -1;
	}
	offset += 2;
	printf("got version number: %d\n", ver);

	if (buf_gets64(data, offset, len, &name) == -1) {
		return -1;
	}
	offset += 8;
	printf("got username: %s\n",
	    mod37_namedec(name, namestr));

	if (server_has_player(name)) {
		net_login_response(p, RESP_ACCOUNT_USED);
		return -1;
	}

	for (int i = 0; i < 3; ++i) {
		uint8_t block_len;
		uint8_t encrypted[128];
		uint8_t decrypted[128];
		int decrypted_len;

		if (buf_getu8(data, offset++, len,
				&block_len) == -1) {
			return -1;
		}
		if (block_len > sizeof(encrypted)) {
			return -1;
		}
		for (size_t j = 0; j < block_len; ++j) {
			if (buf_getu8(data, offset++, len,
					&encrypted[j]) == -1) {
				return -1;
			}
		}
		decrypted_len = rsa_decrypt(&p->mob.server->rsa,
		    encrypted, block_len,
		    decrypted, sizeof(decrypted));
		if (decrypted_len == -1) {
			return -1;
		}
		memcpy(password + (i * 7), decrypted + 8, 7);
	}

	for (size_t i = 0; i < sizeof(password); ++i) {
		if (isspace((unsigned char)password[i])) {
			password[i] = '\0';
			break;
		}
	}
	password[sizeof(password) - 1] = '\0';

	memcpy(p->password, password, sizeof(password));

#if 0
	printf("got password %s\n", password);
#endif

	p->name = name;

	return 0;
}


static int
process_login_isaac(struct player *p, uint8_t *data, size_t offset, size_t len)
{
	uint8_t reconnecting;
	uint8_t limit30;
	uint8_t magic;
	uint16_t revision;
	uint32_t uid;
	uint8_t encrypted[128];
	uint8_t encrypted_len;
	uint8_t decrypted[128];
	char username[21];
	char password[21];
	int64_t name;
	int decrypted_len;

	if (buf_getu8(data, offset++, len, &reconnecting) == -1) {
		return -1;
	}
	if (buf_getu16(data, offset, len, &revision) == -1) {
		return -1;
	}
	offset += 2;

	if (buf_getu8(data, offset++, len, &limit30) == -1) {
		return -1;
	}

	p->protocol_rev = revision;
	printf("got protocol rev %d\n", revision);

	if (buf_getu8(data, offset++, len, &encrypted_len) == -1) {
		return -1;
	}
	if (encrypted_len > sizeof(encrypted)) {
		return -1;
	}
	for (size_t i = 0; i < encrypted_len; ++i) {
		if (buf_getu8(data, offset++, len, &encrypted[i]) == -1) {
			return -1;
		}
	}
	decrypted_len = rsa_decrypt(&p->mob.server->rsa,
	    encrypted, encrypted_len,
	    decrypted, sizeof(decrypted));
	if (decrypted_len == -1) {
		return -1;
	}

	offset = 0;

	if (buf_getu8(decrypted, offset++, decrypted_len, &magic) == -1) {
		return -1;
	}

	/* this is used to verify that RSA decryption succeeded */
	if (magic != 10) {
		net_login_response(p, RESP_FULL);
		return -1;
	}

	for (int i = 0; i < 4; ++i) {
		uint32_t val;

		if (buf_getu32(decrypted, offset, decrypted_len, &val) == -1) {
			return -1;
		}
		offset += 4;

		p->isaac_in.randrsl[i] = val;
		p->isaac_out.randrsl[i] = val;
	}

	isaac_init(&p->isaac_in, 1);
	isaac_init(&p->isaac_out, 1);

	p->isaac_ready = true;

	if (buf_getu32(decrypted, offset, decrypted_len, &uid) == -1) {
		return -1;
	}
	offset += 4;

	for (size_t i = 0; i < (sizeof(username) - 1); ++i) {
		if (buf_getu8(decrypted, offset + i, decrypted_len,
				(uint8_t *)&username[i]) == -1) {
			return -1;
		}
		if (isspace((unsigned char)username[i])) {
			username[i] = '\0';
			break;
		}
	}

	username[sizeof(username) - 1] = '\0';
	offset += sizeof(username);

	name = mod37_nameenc(username);

	if (server_has_player(name)) {
		net_login_response(p, RESP_ACCOUNT_USED);
		return -1;
	}

	p->name = name;

	for (size_t i = 0; i < (sizeof(password) - 1); ++i) {
		if (buf_getu8(decrypted, offset + i, decrypted_len,
				(uint8_t *)&password[i]) == -1) {
			return -1;
		}
		if (isspace((unsigned char)password[i])) {
			password[i] = '\0';
			break;
		}
	}

	password[sizeof(password) - 1] = '\0';
	offset += sizeof(password);

	memcpy(p->password, password, sizeof(password));

	printf("got username %s\n", username);
#if 0
	printf("got password %s\n", password);
#endif

	return 0;
}

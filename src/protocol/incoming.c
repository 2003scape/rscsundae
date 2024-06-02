#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opcodes.h"
#include "utility.h"
#include "../config/anim.h"
#include "../buffer.h"
#include "../entity.h"
#include "../netio.h"
#include "../server.h"

#define MAX_PACKETS_PER_TICK		(10)
#define MAX_PLAYER_HAIR_COLOUR		(9)
#define MAX_PLAYER_SKIN_COLOUR		(4)
#define MAX_PLAYER_CLOTHING_COLOUR	(14)

static void
process_packet(struct player *, uint8_t *, size_t);

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
	uint8_t opcode = data[offset++] & 0xff;

	printf("process packet opcode %d len %d\n", opcode, len);

	p->last_packet = p->mob.server->tick_counter;

	switch (opcode) {
	case OP_CLI_LOGIN:
	case OP_CLI_RECONNECT:
		{
			char namestr[64];
			uint16_t ver;
			int64_t name;

			if (p->name != -1) {
				return;
			}
			if (buf_getu16(data, offset, len, &ver) == -1) {
				return;
			}
			offset += 2;
			printf("got version number: %d\n", ver);

			if (buf_gets64(data, offset, len, &name) == -1) {
				return;
			}
			offset += 8;
			printf("got username: %lld %s\n", name, mod37_namedec(name, namestr));

			if (server_has_player(name)) {
				p->logout_confirmed = true;
				net_login_response(p->sock, RESP_ACCOUNT_USED);
				return;
			}

			p->name = name;

			player_send_design_ui(p);
			player_send_message(p, "@que@Welcome to RSCSundae!");
			player_send_client_settings(p);
			player_send_privacy_settings(p);
			player_send_init_friends(p);
			player_send_init_ignore(p);

			server_register_login(p->name);
			net_login_response(p->sock, RESP_LOGIN_OK);
		}
		break;
	case OP_CLI_LOGOUT:
		{
			if (p->mob.in_combat || p->mob.server->tick_counter <
			    (p->mob.combat_timer + 17)) {
				player_send_logout_reject(p);
				return;
			}

			player_send_logout(p);
		}
		break;
	case OP_CLI_PUBLIC_CHAT:
		{
			/* want to eventually decode this to moderate */
			size_t msglen = len - 1;
			if (msglen <= MAX_PUBLIC_CHAT_LEN) {
				memcpy(p->public_chat_enc, data + offset, msglen);
				p->public_chat_len = msglen;
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

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			printf("add friend %lld\n", target);
			player_add_friend(p, target);
		}
		break;
	case OP_CLI_REMOVE_FRIEND:
		{
			int64_t target;

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			player_remove_friend(p, target);
		}
		break;
	case OP_CLI_PRIVATE_MESSAGE:
		{
			int64_t target;

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			server_send_pm(p, target, data + offset, len - 9);
		}
		break;
	case OP_CLI_ADD_IGNORE:
		{
			int64_t target;

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			player_add_ignore(p, target);
		}
		break;
	case OP_CLI_REMOVE_IGNORE:
		{
			int64_t target;

			if (buf_gets64(data, offset, len, &target) == -1) {
				return;
			}
			offset += 8;
			player_remove_ignore(p, target);
		}
		break;
	case OP_CLI_PRIVACY_SETTINGS:
		{
			uint8_t hide_online;
			uint8_t block_public;
			uint8_t block_private;
			uint8_t block_trade;
			uint8_t block_duel;
			bool unhide = false;

			if (buf_getu8(data, offset++, len, &hide_online) == -1) {
				return;
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
	case OP_CLI_PRAYER_OFF:
		{
			uint8_t id;

			if (buf_getu8(data, offset++, len, &id) == -1) {
				return;
			}
			player_prayer_disable(p, id);
		}
		break;
	case OP_CLI_PRAYER_ON:
		{
			uint8_t id;

			if (buf_getu8(data, offset++, len, &id) == -1) {
				return;
			}
			player_prayer_enable(p, id);
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
			if (id <= MAX_CLIENT_SETTINGS) {
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
			p->following_player = (int16_t)target;
		}
		break;
	case OP_CLI_ATTACK_PLAYER:
		{
			struct player *t;
			uint16_t id;

			if (buf_getu16(data, offset, len, &id) == -1) {
				return;
			}
			offset += 2;
			if (id < MAXPLAYERS) {
				t = p->mob.server->players[id];
				if (t != NULL) {
					player_pvp_attack(p, t);
				}
			}
		}
		break;
	case OP_CLI_COMBAT_STYLE:
		{
			uint8_t style;

			if (buf_getu8(data, offset, len, &style) == -1) {
				return;
			}
			if (style <= COMBAT_STYLE_DEFENSIVE) {
				p->combat_style = style;
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
			player_unwear(p, slot);
		}
		break;
	case OP_CLI_WALK_TILE:
	case OP_CLI_WALK_ENTITY:
		{
			size_t steps;
			int start_x, start_y;

			if (len < 5 ||
			    (p->mob.in_combat && player_retreat(p) == -1)) {
				return;
			}
			steps = (len - 5) / 2;
			if (buf_getu16(data, offset, len, &p->walk_queue_x[0]) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &p->walk_queue_y[0]) == -1) {
				return;
			}
			offset += 2;
			start_x = p->walk_queue_x[0];
			start_y = p->walk_queue_y[0];
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
				p->walk_queue_x[i + 1] = (uint16_t)new_x;
				p->walk_queue_y[i + 1] = (uint16_t)new_y;
			}
			if (opcode != OP_CLI_WALK_ENTITY) {
				p->mob.target_npc = -1;
				p->mob.target_player = -1;
			}
			p->walk_queue_len = steps + 1;
			p->walk_queue_pos = 0;
			p->following_player = -1;
		}
		break;
	case OP_CLI_ACCEPT_DESIGN:
		{
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

			(void)buf_getu8(data, offset++, len, &p->rpg_class);

			player_recalculate_sprites(p);
			p->appearance_changed = true;
			p->ui_design_open = false;
		}
		break;
	}
}

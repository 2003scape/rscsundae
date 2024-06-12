#include <sys/socket.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "config/anim.h"
#include "protocol/opcodes.h"
#include "inventory.h"
#include "server.h"
#include "entity.h"
#include "loop.h"
#include "netio.h"
#include "script.h"
#include "stat.h"
#include "utility.h"
#include "zone.h"

static void player_restore_stat(struct player *, int);
static void player_restore_stats(struct player *);
static int player_get_attack_boosted(struct player *);
static int player_get_defense_boosted(struct player *);
static int player_get_strength_boosted(struct player *);
static int player_pvp_roll(struct player *, struct player *);
static int player_pvp_ranged_roll(struct player *, struct player *);
static void player_process_ranged_pvp(struct player *, struct player *);
static void player_recalculate_bonus(struct player *);

struct player *
player_accept(struct server *s, int sock)
{
	int slot = -1;

	int32_t session_id = ranval(&s->ran);

	/* this has to happen before a slot is selected. */

	(void)send(sock, &session_id, sizeof(session_id), 0);

	/* try to find space for the player */

	for (int i = 0; i < MAXPLAYERS; ++i) {
		if (s->players[i] == NULL) {
			slot = i;
			break;
		}
	}

	if (slot >= s->max_player_id) {
		s->max_player_id = slot + 1;
		printf("set max player id %d\n", s->max_player_id);
	}

	if (slot == -1) {
		/* server is full */
		net_login_response(sock, RESP_FULL);
		return NULL;
	}

	struct player *p = calloc(1, sizeof(struct player));
	if (p == NULL) {
		net_login_response(sock, RESP_FULL);
		return NULL;
	}

	p->name = -1;
	p->mob.id = (uint16_t)slot;
	p->session_id = session_id;
	p->sock = sock;

	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		p->mob.cur_stats[i] = 1;
		p->mob.base_stats[i] = 1;
	}
	p->mob.cur_stats[SKILL_HITS] = 10;
	p->mob.base_stats[SKILL_HITS] = 10;
	p->experience[SKILL_HITS] = 4000;

	/* XXX: testing, remove later... */
	p->mob.cur_stats[SKILL_PRAYER] = 70;
	p->mob.base_stats[SKILL_PRAYER] = 70;

	p->sprites_base[ANIM_SLOT_HEAD] = ANIM_HEAD1 + 1;
	p->sprites_base[ANIM_SLOT_BODY] = ANIM_BODY1 + 1;
	p->sprites_base[ANIM_SLOT_LEGS] = ANIM_LEGS1 + 1;

	p->hair_colour = COLOUR_HAIR_DEFAULT;
	p->top_colour = COLOUR_TOP_DEFAULT;
	p->leg_colour = COLOUR_LEG_DEFAULT;

	p->bonus_weaponaim = 1;
	p->bonus_weaponpower = 1;
	p->bonus_armour = 1;
	p->bonus_prayer = 1;
	p->bonus_magic = 1;

	/* add some test items */
	p->inventory[p->inv_count].id = 10;
	p->inventory[p->inv_count++].stack = 1000;
	p->inventory[p->inv_count].id = 81;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 222;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 5;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 6;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 7;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 9;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 312;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 2;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 24;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 187;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 191;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 188;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 229;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 16;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 17;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 59;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 215;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 0;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 317;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 185;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 184;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 11;
	p->inventory[p->inv_count++].stack = 25;
	p->inventory[p->inv_count].id = 190;
	p->inventory[p->inv_count++].stack = 25;
	p->inventory[p->inv_count].id = 20;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 20;
	p->inventory[p->inv_count++].stack = 1;
	p->inventory[p->inv_count].id = 20;
	p->inventory[p->inv_count++].stack = 1;

	p->stats_changed = true;
	p->bonus_changed = true;
	p->plane_changed = true;
	p->inv_changed = true;
	p->ui_design_open = true;
	p->following_player = -1;
	p->trading_player = -1;
	p->projectile_sprite = UINT16_MAX;
	p->bubble_id = UINT16_MAX;
	p->last_packet = s->tick_counter;

	player_recalculate_combat_level(p);
	player_recalculate_sprites(p);

	p->mob.server = s;
	p->mob.x = s->start_tile_x;
	p->mob.y = s->start_tile_y;
	p->mob.damage = UINT8_MAX;

	mob_combat_reset(&p->mob);
	s->players[slot] = p;

	loop_add_player(p);
	return p;
}

void
player_process_walk_queue(struct player *p)
{
	if (p->mob.in_combat) {
		if (p->mob.dir == MOB_DIR_COMBAT_LEFT ||
		    p->mob.dir == MOB_DIR_COMBAT_RIGHT) {
			p->walk_queue_len = 0;
			p->walk_queue_pos = 0;
			return;
		}
	}
	if (p->following_player != -1) {
		struct player *p2;

		p2 = p->mob.server->players[p->following_player];
		if (p2 != NULL) {
			if (mob_within_range(&p->mob, p2->mob.x, p2->mob.y, 2)) {
				return;
			}
			if (!mob_within_range(&p->mob, p2->mob.x, p2->mob.y, 3)) {
				p->walk_queue_x[0] = p2->mob.x;
				p->walk_queue_y[0] = p2->mob.y;
				p->walk_queue_pos = 0;
				p->walk_queue_len = 1;
			}
		} else {
			p->following_player = -1;
		}
	}
	int pos = p->walk_queue_pos;
	int remaining = p->walk_queue_len - pos;
	if (remaining == 0) {
		p->walk_queue_pos = 0;
		p->walk_queue_len = 0;
		return;
	}
	int cur_x = p->mob.x;
	int cur_y = p->mob.y;
	int dif_x = cur_x - (int)p->walk_queue_x[pos];
	int dif_y = cur_y - (int)p->walk_queue_y[pos];

	if (dif_x == 0) {
		if (dif_y > 0) {
			p->mob.dir = MOB_DIR_NORTH;
			p->mob.y = cur_y - 1;
		} else if (dif_y < 0) {
			p->mob.dir = MOB_DIR_SOUTH;
			p->mob.y = cur_y + 1;
		}
	} else if (dif_x < 0) {
		if (dif_y == 0) {
			p->mob.dir = MOB_DIR_WEST;
			p->mob.x = cur_x + 1;
		} else if (dif_y < 0) {
			p->mob.dir = MOB_DIR_SOUTHWEST;
			p->mob.x = cur_x + 1;
			p->mob.y = cur_y + 1;
		} else if (dif_y > 0) {
			p->mob.dir = MOB_DIR_NORTHWEST;
			p->mob.x = cur_x + 1;
			p->mob.y = cur_y - 1;
		}
	} else if (dif_x > 0) {
		if (dif_y == 0) {
			p->mob.dir = MOB_DIR_EAST;
			p->mob.x = cur_x - 1;
		} else if (dif_y < 0) {
			p->mob.dir = MOB_DIR_SOUTHEAST;
			p->mob.x = cur_x - 1;
			p->mob.y = cur_y + 1;
		} else if (dif_y > 0) {
			p->mob.dir = MOB_DIR_NORTHEAST;
			p->mob.x = cur_x - 1;
			p->mob.y = cur_y - 1;
		}
	}

	if (p->mob.x == p->walk_queue_x[pos] &&
	    p->mob.y == p->walk_queue_y[pos]) {
		p->walk_queue_pos++;
	}

	if (p->mob.x != cur_x || p->mob.y != cur_y) {
		player_close_ui(p);
		p->moved = true;
	}

}

void
player_close_ui(struct player *p)
{
	if (p->ui_trade_open) {
		player_send_close_trade(p);
	}
	if (p->ui_multi_open) {
		player_send_hide_multi(p);
	}
	p->trade_state = TRADE_STATE_NONE;
	p->trading_player = -1;
	p->offer_count = 0;
	p->ui_design_open = false;
	p->ui_trade_open = false;
}

void
player_destroy(struct player *p)
{
	if (p->sock != -1) {
		close(p->sock);
		p->sock = -1;
	}
	free(p);
}

bool
player_has_friend(struct player *p, int64_t name)
{
	for (int i = 0; i < p->friend_count; ++i) {
		if (p->friend_list[i] == name) {
			return true;
		}
	}
	return false;
}

bool
player_has_ignore(struct player *p, int64_t name)
{
	for (int i = 0; i < p->ignore_count; ++i) {
		if (p->ignore_list[i] == name) {
			return true;
		}
	}
	return false;
}

int
player_add_friend(struct player *p, int64_t entry)
{
	struct player *p2;

	if (p->friend_count >= MAX_FRIENDS) {
		return -1;
	}
	p->friend_list[p->friend_count++] = entry;
	p2 = server_find_player_name37(entry);
	if (p2 != NULL) {
		player_notify_friend_online(p, entry);
		player_notify_friend_online(p2, p->name);
	} else {
		player_notify_friend_offline(p, entry);
	}
	return 0;
}

int
player_remove_friend(struct player *p, int64_t entry)
{
	for (int i = 0; i < p->friend_count; ++i) {
		if (p->friend_list[i] != entry) {
			continue;
		}
		if (p->block_private) {
			struct player *p2;

			p2 = server_find_player_name37(entry);
			if (p2 != NULL) {
				player_notify_friend_offline(p2, p->name);
			}
		}
		if (i != (p->friend_count - 1)) {
			memmove(p->friend_list + i,
				p->friend_list + i + 1,
				sizeof(int64_t) * (p->friend_count - i - 1));
		}
		p->friend_count--;
		break;
	}
	return 0;
}

int
player_add_ignore(struct player *p, int64_t entry)
{
	if (p->ignore_count >= MAX_IGNORE) {
		return -1;
	}
	if (!p->block_private || player_has_friend(p, entry)) {
		struct player *p2;

		p2 = server_find_player_name37(entry);
		if (p2 != NULL) {
			player_notify_friend_offline(p2, p->name);
		 }
	}
	p->ignore_list[p->ignore_count++] = entry;
	return 0;
}

int
player_remove_ignore(struct player *p, int64_t entry)
{
	for (int i = 0; i < p->ignore_count; ++i) {
		struct player *p2;

		if (p->ignore_list[i] != entry) {
			continue;
		}
		if (i != (p->ignore_count - 1)) {
			memmove(p->ignore_list + i,
				p->ignore_list + i + 1,
				sizeof(int64_t) * (p->ignore_count - i - 1));
		}
		p->ignore_count--;
		p2 = server_find_player_name37(entry);
		if (p2 != NULL) {
			player_notify_friend_online(p2, p->name);
		}
		break;
	}
	return 0;
}

bool
player_is_blocked(struct player *p, int64_t speaker, bool flag)
{
	if (player_has_ignore(p, speaker)) {
		return true;
	}
	if (player_has_friend(p, speaker)) {
		return false;
	}
	return flag;
}

static int
player_get_attack_boosted(struct player *p)
{
	int stat = 8;

	switch (p->combat_style) {
	case COMBAT_STYLE_ACCURATE:
		stat += 3;
		break;
	case COMBAT_STYLE_CONTROLLED:
		stat += 1;
		break;
	}

	if (p->prayers[PRAY_INCREDIBLE_REFLEXES]) {
		stat += (p->mob.cur_stats[SKILL_ATTACK] * 1.15);
	} else if (p->prayers[PRAY_IMPROVED_REFLEXES]) {
		stat += (p->mob.cur_stats[SKILL_ATTACK] * 1.10);
	} else if (p->prayers[PRAY_CLARITY_OF_THOUGHT]) {
		stat += (p->mob.cur_stats[SKILL_ATTACK] * 1.05);
	} else {
		stat += p->mob.cur_stats[SKILL_ATTACK];
	}
	return stat;
}

static int
player_get_defense_boosted(struct player *p)
{
	int stat = 8;

	switch (p->combat_style) {
	case COMBAT_STYLE_DEFENSIVE:
		stat += 3;
		break;
	case COMBAT_STYLE_CONTROLLED:
		stat += 1;
		break;
	}

	if (p->prayers[PRAY_STEEL_SKIN]) {
		stat += (p->mob.cur_stats[SKILL_DEFENSE] * 1.15);
	} else if (p->prayers[PRAY_ROCK_SKIN]) {
		stat += (p->mob.cur_stats[SKILL_DEFENSE] * 1.10);
	} else if (p->prayers[PRAY_THICK_SKIN]) {
		stat += (p->mob.cur_stats[SKILL_DEFENSE] * 1.05);
	} else {
		stat += p->mob.cur_stats[SKILL_DEFENSE];
	}
	return stat;
}

static int
player_get_strength_boosted(struct player *p)
{
	int stat = 8;

	switch (p->combat_style) {
	case COMBAT_STYLE_AGGRESSIVE:
		stat += 3;
		break;
	case COMBAT_STYLE_CONTROLLED:
		stat += 1;
		break;
	}

	if (p->prayers[PRAY_ULTIMATE_STRENGTH]) {
		stat += (p->mob.cur_stats[SKILL_STRENGTH] * 1.15);
	} else if (p->prayers[PRAY_SUPERHUMAN_STRENGTH]) {
		stat += (p->mob.cur_stats[SKILL_STRENGTH] * 1.10);
	} else if (p->prayers[PRAY_BURST_OF_STRENGTH]) {
		stat += (p->mob.cur_stats[SKILL_STRENGTH] * 1.05);
	} else {
		stat += p->mob.cur_stats[SKILL_STRENGTH];
	}
	return stat;
}

static int
player_pvp_ranged_roll(struct player *attacker, struct player *defender)
{
	int def = player_get_defense_boosted(defender);

	assert(attacker->projectile != NULL);

	return mob_combat_roll(&attacker->mob.server->ran,
	    8 + attacker->mob.cur_stats[SKILL_RANGED],
	    attacker->projectile->aim,
	    def, defender->bonus_armour,
	    8 + attacker->mob.cur_stats[SKILL_RANGED],
	    attacker->projectile->power);
}

static int
player_pvp_roll(struct player *attacker, struct player *defender)
{
	int att = player_get_attack_boosted(attacker);
	int def = player_get_defense_boosted(defender);
	int str = player_get_strength_boosted(attacker);

	return mob_combat_roll(&attacker->mob.server->ran,
	    att, attacker->bonus_weaponaim,
	    def, defender->bonus_armour,
	    str, attacker->bonus_weaponpower);
}

void
player_pvp_attack(struct player *attacker, struct player *target)
{
	if (attacker->mob.in_combat) {
		player_send_message(attacker,
		    "You are already busy fighting!");
		return;
	}
	attacker->mob.target_player = target->mob.id;
}

void
player_die(struct player *p, struct player *victor)
{
	struct item_config *item;
	int kept_max = 3;

	/* TODO teleportation */
	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		p->mob.cur_stats[i] = p->mob.base_stats[i];
	}

	p->mob.damage_timer = 0;
	p->mob.combat_timer = 0;
	p->walk_queue_len = 0;
	p->walk_queue_pos = 0;
	player_clear_actions(p);

	mob_combat_reset(&p->mob);

	item = server_find_item_config("bones");
	assert(item != NULL);
	server_add_temp_item(victor, p->mob.x, p->mob.y, item->id, 1);

	if (p->skulled) {
		kept_max = 0;
		p->skulled = false;
	}

	if (p->prayers[PRAY_PROTECT_ITEM]) {
		kept_max++;
	}

	while (p->inv_count > kept_max) {
		int slot = INT_MAX;
		uint32_t lowest_value = UINT32_MAX;

		for (int i = 0; i < p->inv_count; ++i) {
			item = server_item_config_by_id(p->inventory[i].id);
			if (item->value < lowest_value) {
				slot = i;
				lowest_value = item->value;
			}
		}
		if (slot >= p->inv_count) {
			break;
		}
		item = server_item_config_by_id(p->inventory[slot].id);
		if (item->weight == 0) {
			player_inv_remove(p, item,
			    p->inventory[slot].stack);
			server_add_temp_item(victor,
			    p->mob.x, p->mob.y, item->id,
				p->inventory[slot].stack);
		} else {
			player_inv_remove(p, item, 1);
			server_add_temp_item(victor,
			    p->mob.x, p->mob.y, item->id, 1);
		}
	}

	for (int i = 0; i < p->inv_count; ++i) {
		struct item_config *item =
		    server_item_config_by_id(p->inventory[i].id);
		if (item->weight == 0 && p->inventory[i].stack > 1) {
			player_inv_remove(p, item,
			    p->inventory[i].stack - 1);
		}
	}

	player_reset_prayers(p);

	player_send_death(p);
	p->appearance_changed = true;
}

static void
player_process_ranged_pvp(struct player *p, struct player *target)
{
	struct item_config *ammo_config;
	char name[32], message[64];
	int range;
	int roll;

	assert(p != NULL);
	assert(target != NULL);
	assert(p->projectile != NULL);

	range = p->projectile->range;

	p->walk_queue_len = 0;
	p->walk_queue_pos = 0;
	p->following_player = -1;

	if (p->mob.server->tick_counter < p->mob.combat_next_hit) {
		return;
	}

	if ((abs(p->mob.x - (int)target->mob.x) > range) ||
	    (abs(p->mob.y - (int)target->mob.y) > range)) {
		p->following_player = target->mob.id;
		return;
	}

	if (target->prayers[PRAY_PROTECT_FROM_MISSILES]) {
		player_send_message(p,
		    "Player has a protection from missiles prayer active!");
		p->mob.target_player = -1;
		return;
	}

	ammo_config = server_find_item_config(p->projectile->item);
	if (ammo_config != NULL) {
		if (player_inv_held(p, ammo_config, 1)) {
			player_inv_remove(p, ammo_config, 1);
		} else {
			player_send_message(p, "I've run out of ammo!");
			p->mob.target_player = -1;
			return;
		}
	}

	/* TODO: verify reachability */

	/* XXX verify if it's always northwest */
	p->mob.dir = MOB_DIR_NORTHWEST;

	if (!p->skulled) {
		/* skull remains for 20 minutes */
		/* TODO: should track players who attacked us */
		p->skulled = true;
		p->skull_timer =
		    p->mob.server->tick_counter + 2000;
		p->appearance_changed = true;
	}

	roll = player_pvp_ranged_roll(p, target);
	if (roll > 0) {
		stat_advance(p, SKILL_RANGED, roll * 16, 0);
	}
	if (roll >= target->mob.cur_stats[SKILL_HITS]) {
		char name[32], msg[64];

		p->mob.target_player = -1;
		player_die(target, p);
		mod37_namedec(target->name, name);
		(void)snprintf(msg, sizeof(msg),
		    "You have defeated %s!", name);
		player_send_message(p, msg);
		return;
	}

	target->mob.cur_stats[SKILL_HITS] -= roll;
	target->mob.damage = roll;
	target->mob.damage_timer = p->mob.server->tick_counter;

	p->projectile_sprite = p->projectile->sprite;
	p->projectile_target_player = target->mob.id;

	mod37_namedec(p->name, name);
	(void)snprintf(message, sizeof(message),
	    "Warning! %s is shooting at you!", name);
	player_send_message(target, message);

	p->mob.combat_next_hit = p->mob.server->tick_counter + 4;
}

void
player_process_combat(struct player *p)
{

	if (!p->mob.in_combat) {
		if (p->mob.target_player != -1) {
			struct player *target;

			target = p->mob.server->players[p->mob.target_player];
			if (target == NULL) {
				mob_combat_reset(&p->mob);
				return;
			}

			int depth = mob_wilderness_level(&target->mob);
			if (depth <= 0) {
				mob_combat_reset(&p->mob);
				return;
			}

			int difference = abs(target->mob.combat_level -
			    (int)p->mob.combat_level);

			if (difference > depth) {
				char msgdepth[64];

				(void)snprintf(msgdepth, sizeof(msgdepth),
				    "You can only attack players within %d levels of your own here",
				    depth);
				player_send_message(p, msgdepth);
				player_send_message(p, "Move further into the wilderness for less restrictions");
				mob_combat_reset(&p->mob);
				return;
			}

			if (p->projectile != NULL) {
				player_process_ranged_pvp(p, target);
				return;
			}

			if (p->mob.server->tick_counter <
			    (target->mob.combat_timer + 6)) {
				/*
				 * hp bar in client takes roughly 4 seconds
				 * to be gone
				 */
				p->walk_queue_pos = 0;
				p->walk_queue_len = 0;
				mob_combat_reset(&p->mob);
				return;
			}

			if (target->mob.in_combat) {
				/* XXX needs verifying */
				player_send_message(p, "I can't get close enough");
				p->walk_queue_pos = 0;
				p->walk_queue_len = 0;
				mob_combat_reset(&p->mob);
				return;
			}

			if (!mob_within_range(&p->mob,
			    target->mob.x, target->mob.y, 3)) {
				return;
			}

			if (p->mob.x == target->mob.x &&
			    p->mob.y == target->mob.y) {
				p->mob.dir = MOB_DIR_COMBAT_RIGHT;
				p->walk_queue_len = 0;
				p->walk_queue_pos = 0;
			} else {
				/* TODO reachability */
				p->walk_queue_x[0] = target->mob.x;
				p->walk_queue_y[0] = target->mob.y;
				p->walk_queue_len = 1;
				p->walk_queue_pos = 0;
			}

			/* successful catch, combat lock the target */
			target->walk_queue_len = 0;
			target->walk_queue_pos = 0;

			if (!p->skulled) {
				/* skull remains for 20 minutes */
				/* TODO: should track players who attacked us */
				p->skulled = true;
				p->skull_timer =
				    p->mob.server->tick_counter + 2000;
				p->appearance_changed = true;
			}

			player_close_ui(p);
			player_clear_actions(p);
			p->mob.target_player = target->mob.id;
			p->mob.target_npc = -1;
			p->mob.in_combat = true;
			p->mob.combat_next_hit = 0;
			p->mob.combat_rounds = 0;

			player_close_ui(target);
			player_clear_actions(target);
			target->walk_queue_len = 0;
			target->walk_queue_pos = 0;
			target->mob.target_player = p->mob.id;
			target->mob.target_npc = -1;
			target->mob.in_combat = true;
			target->mob.combat_next_hit =
			    target->mob.server->tick_counter + 3;
			target->mob.combat_rounds = 0;
			target->mob.dir = MOB_DIR_COMBAT_LEFT;

			player_send_message(target, "You are under attack!");
		}
		return;
	}

	if (p->mob.server->tick_counter < p->mob.combat_next_hit) {
		return;
	}

	if (p->mob.target_player != -1) {
		struct player *target;
		int roll;

		target = p->mob.server->players[p->mob.target_player];
		if (target == NULL) {
			mob_combat_reset(&p->mob);
			return;
		}

		if (p->mob.x != target->mob.x ||
		    p->mob.y != target->mob.y) {
			return;
		}

		/*
		 * assume the target was "combat locked" as we were running
		 * towards.  this allows the smooth "glide" into combat after
		 * a successful catch.
		 */
		if (p->mob.dir != MOB_DIR_COMBAT_RIGHT &&
		    p->mob.dir != MOB_DIR_COMBAT_LEFT) {
			p->mob.dir = MOB_DIR_COMBAT_RIGHT;
			target->mob.dir = MOB_DIR_COMBAT_LEFT;
		}

		roll = player_pvp_roll(p, target);
		if (roll >= target->mob.cur_stats[SKILL_HITS]) {
			char name[32], msg[64];

			mob_combat_reset(&p->mob);
			player_award_combat_xp(p, &target->mob);
			player_die(target, p);
			mod37_namedec(target->name, name);
			(void)snprintf(msg, sizeof(msg),
			    "You have defeated %s!", name);
			player_send_message(p, msg);
			return;
		}
		target->mob.cur_stats[SKILL_HITS] -= roll;
		target->mob.damage = roll;
		target->mob.combat_rounds++;
		target->mob.combat_timer = p->mob.server->tick_counter;
		target->mob.damage_timer = p->mob.server->tick_counter;
	}

	p->mob.combat_next_hit = p->mob.server->tick_counter + 4;
}

int
player_retreat(struct player *p)
{
	struct player *p2;

	if (p->mob.combat_rounds < 3) {
		player_send_message(p,
		    "You can't retreat during the first 3 rounds of combat");
		return -1;
	}

	if (p->mob.target_player != -1) {
		p2 = p->mob.server->players[p->mob.target_player];
		if (p2 != NULL) {
			player_send_message(p2,
			    "Your opponent is retreating!");
			p2->walk_queue_len = 0;
			p2->walk_queue_pos = 0;
			mob_combat_reset(&p2->mob);
		}
	}

	mob_combat_reset(&p->mob);
	return 0;
}

static void
player_recalculate_bonus(struct player *p)
{
	struct item_config *item;
	struct projectile_config *proj;
	int orig_armour = p->bonus_armour;
	int orig_aim = p->bonus_weaponaim;
	int orig_power = p->bonus_weaponpower;
	int orig_magic = p->bonus_magic;
	int orig_prayer = p->bonus_prayer;

	p->bonus_armour = 1;
	p->bonus_weaponaim = 1;
	p->bonus_weaponpower = 1;
	p->bonus_magic = 1;
	p->bonus_prayer = 1;
	p->projectile = NULL;

	for (int i = 0; i < p->inv_count; ++i) {
		if (!p->inventory[i].worn) {
			continue;
		}
		item = server_item_config_by_id(p->inventory[i].id);
		assert(item != NULL);
		proj = server_find_projectile(item->projectile);
		if (proj != NULL) {
			p->projectile = proj;
		}
		p->bonus_armour += item->bonus_armour;
		p->bonus_weaponaim += item->bonus_aim;
		p->bonus_weaponpower += item->bonus_power;
		p->bonus_magic += item->bonus_magic;
		p->bonus_prayer += item->bonus_prayer;
	}

	if (p->bonus_weaponaim != orig_aim ||
	    p->bonus_weaponpower != orig_power ||
	    p->bonus_armour != orig_armour ||
	    p->bonus_magic != orig_magic ||
	    p->bonus_prayer != orig_prayer) {
		p->bonus_changed = true;
	}
}

int
player_wear(struct player *p, int slot)
{
	struct item_config *type, *type2;

	if (slot < 0 || slot >= p->inv_count) {
		return -1;
	}
	type = server_item_config_by_id(p->inventory[slot].id);
	if (type == NULL || type->equip_type == 0) {
		return -1;
	}
	/* XXX should be delayed by tick? */
	for (int i = 0; i < p->inv_count; ++i) {
		if (i == slot || !p->inventory[i].worn) {
			continue;
		}
		type2 = server_item_config_by_id(p->inventory[i].id);
		if (type2 == NULL || type2->equip_type == 0) {
			continue;
		}
		if (item_equip_clear(type, type2->equip_type)) {
			p->inventory[i].worn = false;
			player_send_inv_slot(p, i);
		}
	}
	p->inventory[slot].worn = true;
	player_recalculate_bonus(p);
	player_recalculate_sprites(p);
	player_send_inv_slot(p, slot);
	return 0;
}

int
player_unwear(struct player *p, int slot)
{
	struct item_config *type;

	if (slot < 0 || slot >= p->inv_count) {
		return -1;
	}
	/* XXX should be delayed by tick */
	type = server_item_config_by_id(p->inventory[slot].id);
	if (type == NULL || type->equip_type == 0) {
		return -1;
	}
	p->inventory[slot].worn = false;
	player_recalculate_bonus(p);
	player_recalculate_sprites(p);
	player_send_inv_slot(p, slot);
	return 0;
}

void
player_recalculate_combat_level(struct player *p)
{
	int attack = p->mob.base_stats[SKILL_ATTACK];
	int defense = p->mob.base_stats[SKILL_DEFENSE];
	int strength = p->mob.base_stats[SKILL_STRENGTH];
	int hits = p->mob.base_stats[SKILL_HITS];
	int ranged = p->mob.base_stats[SKILL_RANGED];
	int magic = p->mob.base_stats[SKILL_MAGIC];
	int prayer = p->mob.base_stats[SKILL_PRAYER];
	uint8_t level;

	if ((int)(ranged * 1.5) < (attack + strength)) {
		level = (uint8_t)
		    ((attack * 0.25) + (defense * 0.25) + (strength * 0.25) +
		    (hits * 0.25) + (magic * 0.125) + (prayer * 0.125));
	} else {
		level = (uint8_t)((ranged * 0.375) +
		    (magic * 0.125) + (prayer * 0.125) +
		    (defense * 0.25) + (hits * 0.25));
	}

	if (level != p->mob.combat_level) {
		p->mob.combat_level = level;
		p->appearance_changed = true;
	}
}

void
player_award_combat_xp(struct player *p, struct mob *target)
{
	uint32_t xp;

	assert(target != NULL);

	xp = mob_combat_xp(target);
	switch (p->combat_style) {
	case COMBAT_STYLE_CONTROLLED:
		/* this order has been verified */
		stat_advance(p, SKILL_ATTACK, xp, 0);
		stat_advance(p, SKILL_DEFENSE, xp, 0);
		stat_advance(p, SKILL_STRENGTH, xp, 0);
		break;
	case COMBAT_STYLE_AGGRESSIVE:
		stat_advance(p, SKILL_STRENGTH, xp * 3, 0);
		break;
	case COMBAT_STYLE_ACCURATE:
		stat_advance(p, SKILL_ATTACK, xp * 3, 0);
		break;
	case COMBAT_STYLE_DEFENSIVE:
		stat_advance(p, SKILL_DEFENSE, xp * 3, 0);
		break;
	}

	/* it's been verified that hits is given last */
	stat_advance(p, SKILL_HITS, xp, 0);
}

void
player_recalculate_sprites(struct player *p)
{
	uint8_t sprites_orig[MAX_ENTITY_SPRITES];

	memcpy(sprites_orig, p->sprites, sizeof(sprites_orig));
	memcpy(p->sprites, p->sprites_base, sizeof(p->sprites));

	for (int i = 0; i < p->inv_count; ++i) {
		if (!p->inventory[i].worn) {
			continue;
		}
		struct item_config *item;

		item = server_item_config_by_id(p->inventory[i].id);
		if (item == NULL) {
			continue;
		}
		if (item->equip_type == EQUIP_TYPE_WEAPON_OFF) {
			/* funny special case, bows go in the shield slot */
			p->sprites[ANIM_SLOT_OFFHAND] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_WEAPON) != 0) {
			p->sprites[ANIM_SLOT_HAND] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_SHIELD) != 0) {
			p->sprites[ANIM_SLOT_OFFHAND] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_HEAD) != 0) {
			p->sprites[ANIM_SLOT_HAT] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_TORSO) != 0) {
			p->sprites[ANIM_SLOT_SHIRT] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_HANDS) != 0) {
			p->sprites[ANIM_SLOT_GLOVES] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_LEGS) != 0) {
			p->sprites[ANIM_SLOT_TROUSERS] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_SHOES) != 0) {
			p->sprites[ANIM_SLOT_SHOES] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_NECK) != 0) {
			p->sprites[ANIM_SLOT_NECK] = item->entity_sprite + 1;
		} else if ((item->equip_type & EQUIP_TYPE_BACK) != 0) {
			p->sprites[ANIM_SLOT_BACK] = item->entity_sprite + 1;
		}

		if (item->equip_type == EQUIP_TYPE_HEAD_FULL) {
			p->sprites[ANIM_SLOT_HEAD] = 0;
		}
		if (item->equip_type == EQUIP_TYPE_TORSO_FULL) {
			p->sprites[ANIM_SLOT_BODY] = 0;
		}
		if (item->equip_type == EQUIP_TYPE_LEGS_FULL) {
			p->sprites[ANIM_SLOT_LEGS] = 0;
		}
	}

	if (memcmp(sprites_orig, p->sprites, sizeof(sprites_orig)) != 0) {
		p->appearance_changed = true;
	}
}

static void
player_restore_stat(struct player *p, int stat)
{
	if (p->mob.cur_stats[stat] < p->mob.base_stats[stat]) {
		p->mob.cur_stats[stat]++;
		player_send_stat(p, stat);
	}
}

static void
player_restore_stats(struct player *p)
{
	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		if (i == SKILL_PRAYER || i == SKILL_HITS) {
			continue;
		}
		player_restore_stat(p, i);
	}
}

void
player_slow_restore(struct player *p)
{
	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		if (p->mob.cur_stats[i] > p->mob.base_stats[i]) {
			p->mob.cur_stats[i]--;
			player_send_stat(p, i);
		}
	}
	if (!p->prayers[PRAY_RAPID_HEAL]) {
		player_restore_stat(p, SKILL_HITS);
	}
	if (!p->prayers[PRAY_RAPID_RESTORE]) {
		player_restore_stats(p);
	}
}

void
player_rapid_restore(struct player *p)
{
	if (p->prayers[PRAY_RAPID_HEAL]) {
		player_restore_stat(p, SKILL_HITS);
	}
	if (p->prayers[PRAY_RAPID_RESTORE]) {
		player_restore_stats(p);
	}
}

void
player_prayer_enable(struct player *p, int prayer)
{
	struct prayer_config *config = NULL;

	if (p->mob.cur_stats[SKILL_PRAYER] == 0 || prayer >= MAX_PRAYERS) {
		return;
	}
	config = server_prayer_config_by_id(prayer);
	assert(config != NULL);
	if (p->mob.base_stats[SKILL_PRAYER] < config->level) {
		return;
	}
	/* do not allow overlapping stat boost prayers */
	switch (prayer) {
	case PRAY_THICK_SKIN:
	case PRAY_ROCK_SKIN:
	case PRAY_STEEL_SKIN:
		p->prayers[PRAY_THICK_SKIN] = false;
		p->prayers[PRAY_ROCK_SKIN] = false;
		p->prayers[PRAY_STEEL_SKIN] = false;
		break;
	case PRAY_BURST_OF_STRENGTH:
	case PRAY_SUPERHUMAN_STRENGTH:
	case PRAY_ULTIMATE_STRENGTH:
		p->prayers[PRAY_BURST_OF_STRENGTH] = false;
		p->prayers[PRAY_SUPERHUMAN_STRENGTH] = false;
		p->prayers[PRAY_ULTIMATE_STRENGTH] = false;
		break;
	case PRAY_CLARITY_OF_THOUGHT:
	case PRAY_IMPROVED_REFLEXES:
	case PRAY_INCREDIBLE_REFLEXES:
		p->prayers[PRAY_CLARITY_OF_THOUGHT] = false;
		p->prayers[PRAY_IMPROVED_REFLEXES] = false;
		p->prayers[PRAY_INCREDIBLE_REFLEXES] = false;
		break;
	}
	p->prayers[prayer] = true;
	p->prayer_drain = 0;
	p->next_drain = 0;
	p->drain_counter = 0;
	for (int i = 0; i < MAX_PRAYERS; ++i) {
		if (p->prayers[i]) {
			config = server_prayer_config_by_id(i);
			assert(config != NULL);
			p->prayer_drain += config->drain;
		}
	}
	p->prayers_changed = true;
}

void
player_reset_prayers(struct player *p)
{
	for (int i = 0; i < MAX_PRAYERS; ++i) {
		if (p->prayers[i]) {
			memset(p->prayers, 0, sizeof(p->prayers));
			p->next_drain = 0;
			p->drain_counter = 0;
			p->prayer_drain = 0;
			p->prayers_changed = true;
			break;
		}
	}
}

void
player_prayer_disable(struct player *p, int prayer)
{
	struct prayer_config *config;

	if (prayer >= MAX_PRAYERS) {
		return;
	}
	if (p->prayers[prayer]) {
		config = server_prayer_config_by_id(prayer);
		assert(config != NULL);
		p->next_drain = 0;
		p->drain_counter = 0;
		p->prayer_drain -= config->drain;
		p->prayers[prayer] = false;
		p->prayers_changed = true;
	}
}

void
player_prayer_drain(struct player *p)
{
	int amount = 1;
	int drain = p->prayer_drain;

	drain -= drain * (p->bonus_prayer * 0.0315);

	if (drain <= 0 || (p->drain_counter++) < p->next_drain) {
		return;
	}

	if (drain >= 60) {
		amount = drain / 60;
	}

	if (amount >= p->mob.cur_stats[SKILL_PRAYER]) {
		player_send_message(p,
		    "You have run out of prayer points. "
		    "Return to a church to recharge");
		p->mob.cur_stats[SKILL_PRAYER] = 0;
		player_reset_prayers(p);
	} else {
		p->mob.cur_stats[SKILL_PRAYER] -= amount;
	}
	player_send_stat(p, SKILL_PRAYER);
	p->drain_counter = 0;
	p->next_drain = drain < 60 ? 60 / drain : 0;
}

bool
player_has_known_loc(struct player *p, int x, int y)
{
	for (size_t i = 0; i < p->known_loc_count; ++i) {
		if (p->known_locs[i].x == x && p->known_locs[i].y == y) {
			return true;
		}
	}
	return false;
}

void
player_add_known_loc(struct player *p, struct loc *loc)
{
	if (p->known_loc_count >= p->known_loc_max) {
		size_t n = p->known_loc_max * 2;
		if (n == 0) {
			n = 128;
		}
		if (reallocarr(&p->known_locs,
		    n, sizeof(struct loc)) == -1) {
			return;
		}
		p->known_loc_max = n;
	}
	memcpy(&p->known_locs[p->known_loc_count++], loc, sizeof(struct loc));
}

void
player_remove_known_loc(struct player *p, size_t index)
{
	p->known_loc_count--;
	for (size_t i = index; i < p->known_loc_count; ++i) {
		p->known_locs[i] = p->known_locs[i + 1];
	}
}

bool
player_has_known_bound(struct player *p, int x, int y, int dir)
{
	for (size_t i = 0; i < p->known_bound_count; ++i) {
		if (p->known_bounds[i].x == x && p->known_bounds[i].y == y &&
		    p->known_bounds[i].dir == dir) {
			return true;
		}
	}
	return false;
}

void
player_add_known_bound(struct player *p, struct bound *bound)
{
	if (p->known_bound_count >= p->known_bound_max) {
		size_t n = p->known_bound_max * 2;
		if (n == 0) {
			n = 32;
		}
		if (reallocarr(&p->known_bounds,
		    n, sizeof(struct bound)) == -1) {
			return;
		}
		p->known_bound_max = n;
	}
	memcpy(&p->known_bounds[p->known_bound_count++],
		bound, sizeof(struct bound));
}

void
player_clear_actions(struct player *p)
{
	p->action = ACTION_NONE;
	p->following_player = -1;
	p->trading_player = -1;
	p->ui_design_open = false;
}

bool
player_can_see_item(struct player *p, struct ground_item *item)
{
	if (item->respawn_time > p->mob.server->tick_counter) {
		return false;
	}
	/* items appear to other players after 1 minute */
	if (p->mob.server->tick_counter > (item->creation_time + 100)) {
		return true;
	}
	return item->owner == p->mob.id || item->owner == UINT16_MAX;
}

bool
player_has_known_item(struct player *p, uint64_t unique_id)
{
	for (size_t i = 0; i < p->known_item_count; ++i) {
		if (p->known_items[i].unique_id == unique_id) {
			return true;
		}
	}
	return false;
}

size_t
player_get_nearby_items(struct player *p,
			struct ground_item *list, size_t max)
{
	size_t count = 0;
	struct zone *orig;
	struct zone *zone;

	orig = server_find_zone(p->mob.x, p->mob.y);
	if (orig == NULL) {
		return 0;
	}

	for (int i = 0; i < orig->item_count && count < max; ++i) {
		if (player_can_see_item(p, &orig->items[i])) {
			list[count++] = orig->items[i];
		}
	}

	for (int x = -3; x < 4; ++x) {
		for (int y = -3; y < 4; ++y) {
			if (x == 0 && y == 0) {
				continue;
			}
			zone = server_get_zone(orig->x + x, orig->y + y,
			    orig->plane);
			if (zone == NULL) {
				continue;
			}
			for (int i = 0; i < zone->item_count &&
			    count < max; ++i) {
				if (player_can_see_item(p, &zone->items[i])) {
					list[count++] = zone->items[i];
				}
			}
		}
	}

	for (size_t i = 0; i < p->mob.server->temp_item_count; ++i) {
		struct ground_item *item;

		if (count >= max) {
			break;
		}
		item = &p->mob.server->temp_items[i];
		if (!player_can_see_item(p, item)) {
			continue;
		}
		zone = server_find_zone(item->x, item->y);
		if (zone == NULL) {
			continue;
		}
		if (abs(zone->x - (int)orig->x) > 3 ||
		    abs(zone->y - (int)orig->y) > 3) {
			continue;
		}
		list[count++] = p->mob.server->temp_items[i];
	}

	return count;
}

void
player_process_action(struct player *p)
{
	struct npc *npc;
	struct item_config *item_config;
	struct ground_item *item;
	uint16_t id;
	uint32_t stack;

	switch (p->action) {
	case ACTION_NPC_TALK:
		npc = p->mob.server->npcs[p->target_npc];
		if (npc == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		if (!mob_within_range(&p->mob, npc->mob.x, npc->mob.y, 2)) {
			return;
		}
		npc->talk_target = p->mob.id;
		p->walk_queue_len = 0;
		p->walk_queue_pos = 0;
		mob_face(&p->mob, npc->mob.x, npc->mob.y);
		script_onnpctalk(p->mob.server->lua, p, npc);
		p->action = ACTION_NONE;
		break;
	case ACTION_INV_DROP:
		if (p->target_slot >= p->inv_count) {
			p->action = ACTION_NONE;
			return;
		}

		if (p->walk_queue_len > 0 || p->ui_trade_open) {
			return;
		}

		id = p->inventory[p->target_slot].id;
		stack = p->inventory[p->target_slot].stack;
		item_config = server_item_config_by_id(id);
		assert(item_config != NULL);
		player_inv_remove(p, item_config, stack);
		server_add_temp_item(p, p->mob.x, p->mob.y, id, stack);
		p->action = ACTION_NONE;
		break;
	case ACTION_INV_USE:
		if (p->target_slot >= p->inv_count) {
			p->action = ACTION_NONE;
			return;
		}
		id = p->inventory[p->target_slot].id;
		item_config = server_item_config_by_id(id);
		script_onuseobj(p->mob.server->lua, p, item_config);
		p->action = ACTION_NONE;
		break;
	case ACTION_ITEM_TAKE:
		if (p->target_item == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		item = server_find_ground_item(p,
		    p->target_item->x, p->target_item->y,
		    p->target_item->id);
		item_config = server_item_config_by_id(p->target_item->id);
		if (item == NULL || item_config == NULL ||
		    p->inv_count >= MAX_INV_SIZE ||
		    !player_can_see_item(p, item)) {
			p->action = ACTION_NONE;
			return;
		}
		if (p->mob.x != item->x || p->mob.y != item->y) {
			/* not reached it yet */
			return;
		}
		player_inv_give(p, item_config, item->stack);
		if (item->respawn) {
			item->respawn_time = p->mob.server->tick_counter +
			    (item_config->respawn_rate / 5);
		} else {
			server_remove_temp_item(item->unique_id);
		}
		p->action = ACTION_NONE;
		break;
	}
}

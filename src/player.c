#include <sys/socket.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "config/anim.h"
#include "config/item.h"
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

static void player_stat_changed(struct player *, int);
static void player_restore_stat(struct player *, int);
static void player_restore_stats(struct player *);
static int player_pvp_roll(struct player *, struct player *);
static int player_pvm_roll(struct player *, struct npc *);
static int player_pvp_ranged_roll(struct player *, struct player *);
static int player_pvm_ranged_roll(struct player *, struct npc *);
static int player_magic_damage_roll(int);
static bool player_wilderness_check(struct player *, struct player *);
static bool player_consume_ammo(struct player *, struct projectile_config *);
static bool player_init_combat(struct player *, struct mob *);
static void player_moved(struct player *, int, int);

struct player *
player_create(struct server *s, int sock)
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

	struct player *p = calloc(1, sizeof(struct player));
	if (p == NULL) {
		return NULL;
	}

	p->sock = sock;

	if (slot == -1) {
		/* server is full */
		net_login_response(p, RESP_FULL);
		return NULL;
	}

	p->name = -1;
	p->session_id = session_id;
	p->mob.id = (uint16_t)slot;
	p->mob.server = s;

	p->trading_player = -1;
	p->chased_by_npc = UINT16_MAX;
	p->projectile_target_player = UINT16_MAX;
	p->projectile_target_npc = UINT16_MAX;
	p->bubble_id = UINT16_MAX;
	p->mob.damage = UINT8_MAX;
	p->last_packet = s->tick_counter;

	p->variables = NULL;
	p->variable_count = 0;
	p->variable_max = 0;

	mob_combat_reset(&p->mob);

	s->players[slot] = p;
	loop_add_player(p);
	return p;
}

int
player_load(struct player *p)
{
	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		p->mob.cur_stats[i] = 1;
		p->mob.base_stats[i] = 1;
	}
	p->mob.cur_stats[SKILL_HITS] = 10;
	p->mob.base_stats[SKILL_HITS] = 10;
	p->experience[SKILL_HITS] = 4000;

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

	p->stats_changed = true;
	p->bonus_changed = true;
	p->plane_changed = true;
	p->inv_changed = true;
	p->ui_design_open = true;

	p->rpg_class = UINT8_MAX;

	player_recalculate_combat_level(p);

	player_teleport(p,
	    p->mob.server->start_tile_x,
	    p->mob.server->start_tile_y);

	return database_load_player(&p->mob.server->database, p);
}

void
player_close_ui(struct player *p)
{
	if (p->ui_bank_open) {
		player_send_close_bank(p);
	}
	if (p->ui_trade_open) {
		player_send_close_trade(p);
	}
	if (p->ui_multi_open) {
		player_send_hide_multi(p);
	}
	if (p->shop != NULL) {
		player_send_close_shop(p);
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
	free(p->variables);
	free(p->known_bounds);
	free(p->known_locs);
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

int
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

int
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

int
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

	return mob_combat_roll(8 + attacker->mob.cur_stats[SKILL_RANGED],
	    attacker->projectile->aim,
	    def, defender->bonus_armour,
	    8 + attacker->mob.cur_stats[SKILL_RANGED],
	    attacker->projectile->power);
}

static int
player_pvm_ranged_roll(struct player *attacker, struct npc *npc)
{
	assert(attacker->projectile != NULL);

	return mob_combat_roll(8 + attacker->mob.cur_stats[SKILL_RANGED],
	    attacker->projectile->aim,
	    npc->mob.cur_stats[SKILL_DEFENSE], 0,
	    8 + attacker->mob.cur_stats[SKILL_RANGED],
	    attacker->projectile->power);
}

static int
player_pvm_roll(struct player *p, struct npc *npc)
{
	int att = player_get_attack_boosted(p);
	int str = player_get_strength_boosted(p);

	return mob_combat_roll(att, p->bonus_weaponaim,
	    npc->mob.cur_stats[SKILL_DEFENSE], 0,
	    str, p->bonus_weaponpower);
}

static int
player_pvp_roll(struct player *attacker, struct player *defender)
{
	int att = player_get_attack_boosted(attacker);
	int def = player_get_defense_boosted(defender);
	int str = player_get_strength_boosted(attacker);

	return mob_combat_roll(att, attacker->bonus_weaponaim,
	    def, defender->bonus_armour,
	    str, attacker->bonus_weaponpower);
}

static int
player_magic_damage_roll(int power)
{
	double dmg = (power + 5.0) * server_random();

	return dmg / 10;
}

void
player_die(struct player *p, struct player *victor)
{
	struct item_config *item;
	int kept_max = 3;

	mob_die(&p->mob);

	p->mob.walk_queue_len = 0;
	p->mob.walk_queue_pos = 0;
	player_clear_actions(p);

	if (victor != NULL && victor->mob.target_player == p->mob.id) {
		mob_combat_reset(&victor->mob);
	}

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

	if (victor != NULL) {
		char name[32], msg[64];

		mod37_namedec(p->name, name);
		(void)snprintf(msg, sizeof(msg),
		    "You have defeated %s!", name);
		player_send_message(victor, msg);
	}

	player_teleport(p,
	    p->mob.server->start_tile_x, p->mob.server->start_tile_y);
}

void
player_damage(struct player *p, struct player *aggressor, int roll)
{
	if (roll >= p->mob.cur_stats[SKILL_HITS]) {
		player_die(p, aggressor);
		return;
	}

	p->mob.cur_stats[SKILL_HITS] -= roll;
	p->mob.damage = roll;
	p->mob.damage_timer = p->mob.server->tick_counter;
}

static bool
player_consume_ammo(struct player *p, struct projectile_config *projectile)
{
	struct item_config *ammo_config;

	ammo_config = server_find_item_config(projectile->item);
	if (ammo_config == NULL) {
		return true;
	}
	if (player_inv_held(p, ammo_config, 1)) {
		player_inv_remove(p, ammo_config, 1);
		return true;
	}
	player_send_message(p, "I've run out of ammo!");
	p->mob.target_player = -1;
	p->mob.target_npc = -1;
	return false;
}

void
player_skull(struct player *p, struct player *target)
{
	/* skull remains for 20 minutes */
	/* TODO: should track players who attacked us */
	(void)target;
	p->skull_timer =
	    p->mob.server->tick_counter + 2000;

	if (!p->skulled) {
		p->skulled = true;
		p->appearance_changed = true;
	}
}

void
player_shoot_pvm(struct player *p, struct projectile_config *projectile,
    struct npc *target)
{
	int roll = 0;

	p->mob.walk_queue_len = 0;
	p->mob.walk_queue_pos = 0;
	p->mob.following_player = -1;
	p->mob.following_npc = -1;

	if (projectile->type != PROJECTILE_TYPE_MAGIC) {
		/* XXX verify if it's always northwest */
		p->mob.dir = MOB_DIR_NORTHWEST;
		if (p->ranged_timer > 0) {
			p->ranged_timer--;
			return;
		}
		p->ranged_timer = 3;
	}

	if (!mob_check_reachable(&p->mob,
	    target->mob.x, target->mob.y, true)) {
		player_send_message(p, "I can't get a clear shot from here");
		return;
	}

	if (!player_consume_ammo(p, projectile)) {
		return;
	}

	if (projectile->type != PROJECTILE_TYPE_MAGIC) {
		roll = player_pvm_ranged_roll(p, target);
		if (roll > 0) {
			stat_advance(p, SKILL_RANGED, roll * 16, 0);
		}
	} else if (projectile->power > 0) {
		roll = player_magic_damage_roll(projectile->power);
	}

	if (target->mob.target_player == -1) {
		target->mob.target_player = p->mob.id;
	}

	if (projectile->power > 0) {
		npc_damage(target, p, roll);
	}

	p->projectile_sprite = projectile->sprite;
	p->projectile_target_npc = target->mob.id;
	p->projectile_target_player = UINT16_MAX;
}

void
player_shoot_pvp(struct player *p, struct projectile_config *projectile,
    struct player *target)
{
	char name[32], message[64];
	int range;
	int roll = 0;

	assert(p != NULL);
	assert(target != NULL);
	assert(projectile != NULL);

	p->mob.walk_queue_len = 0;
	p->mob.walk_queue_pos = 0;
	p->mob.following_player = -1;
	p->mob.following_npc = -1;

	if (projectile->type != PROJECTILE_TYPE_MAGIC) {
		range = projectile->range;
		if ((abs(p->mob.x - (int)target->mob.x) > range) ||
		    (abs(p->mob.y - (int)target->mob.y) > range)) {
			p->mob.following_player = target->mob.id;
			return;
		}

		if (target->prayers[PRAY_PROTECT_FROM_MISSILES]) {
			player_send_message(p,
			    "Player has a protection from missiles prayer active!");
			p->mob.target_player = -1;
			return;
		}
	}

	if (projectile->type != PROJECTILE_TYPE_MAGIC) {
		/* XXX verify if it's always northwest */
		p->mob.dir = MOB_DIR_NORTHWEST;
		if (p->ranged_timer > 0) {
			p->ranged_timer--;
			return;
		}
		p->ranged_timer = 3;
	}

	if (!mob_check_reachable(&p->mob,
	    target->mob.x, target->mob.y, true)) {
		player_send_message(p, "I can't get a clear shot from here");
		return;
	}

	if (!player_consume_ammo(p, projectile)) {
		return;
	}

	player_skull(p, target);

	if (projectile->type != PROJECTILE_TYPE_MAGIC) {
		roll = player_pvp_ranged_roll(p, target);
		if (roll > 0) {
			stat_advance(p, SKILL_RANGED, roll * 16, 0);
		}
	} else if (projectile->power > 0) {
		roll = player_magic_damage_roll(projectile->power);
	}

	if (projectile->power > 0) {
		player_damage(target, p, roll);
	}

	p->projectile_sprite = projectile->sprite;
	p->projectile_target_npc = UINT16_MAX;
	p->projectile_target_player = target->mob.id;

	mod37_namedec(p->name, name);
	(void)snprintf(message, sizeof(message),
	    "Warning! %s is shooting at you!", name);
	player_send_message(target, message);
}

static bool
player_wilderness_check(struct player *p, struct player *target)
{
	int depth = mob_wilderness_level(&target->mob);
	if (depth <= 0) {
		return false;
	}

	int difference = abs(target->combat_level -
	    (int)p->combat_level);

	if (difference > depth) {
		char msgdepth[64];

		if (depth == 1) {
			(void)snprintf(msgdepth, sizeof(msgdepth),
			    "You can only attack players within 1 level of your own here");
		} else {
			(void)snprintf(msgdepth, sizeof(msgdepth),
			    "You can only attack players within %d levels of your own here",
			    depth);
		}
		player_send_message(p, msgdepth);
		player_send_message(p,
		    "Move further into the wilderness for less restrictions");
		return false;
	}

	return true;
}

static bool
player_init_combat(struct player *p, struct mob *target)
{
	if (p->mob.in_combat) {
		player_send_message(p,
		    "You are already busy fighting!");
		return false;
	}

	if (target->in_combat) {
		/* XXX message needs verifying */
		player_send_message(p, "I can't get close enough");
		p->mob.walk_queue_pos = 0;
		p->mob.walk_queue_len = 0;
		mob_combat_reset(&p->mob);
		return false;
	}

	if (!mob_within_range(&p->mob, target->x, target->y, 3)) {
		return false;
	}

	if (!mob_check_reachable(&p->mob, target->x, target->y, false)) {
		player_send_message(p, "I can't get close enough");
		p->mob.walk_queue_pos = 0;
		p->mob.walk_queue_len = 0;
		mob_combat_reset(&p->mob);
		return false;
	}

	if (p->mob.x == target->x &&
	    p->mob.y == target->y) {
		p->mob.dir = MOB_DIR_COMBAT_RIGHT;
		p->mob.walk_queue_len = 0;
		p->mob.walk_queue_pos = 0;
	} else {
		p->mob.action_walk = true;
		p->mob.walk_queue_x[0] = target->x;
		p->mob.walk_queue_y[0] = target->y;
		p->mob.walk_queue_len = 1;
		p->mob.walk_queue_pos = 0;
	}

	/* successful catch, combat lock the target */
	target->walk_queue_len = 0;
	target->walk_queue_pos = 0;
	target->following_player = -1;
	target->following_npc = -1;

	p->mob.following_player = -1;
	p->mob.following_npc = -1;

	player_close_ui(p);
	player_clear_actions(p);

	if (target->moved) {
		/* can't move and enter combat in the same tick */
		/* due to protocol limitations */
		return false;
	}

	p->mob.in_combat = true;
	p->mob.combat_next_hit = 0;
	p->mob.combat_rounds = 0;

	target->walk_queue_len = 0;
	target->walk_queue_pos = 0;
	target->target_player = p->mob.id;
	target->target_npc = -1;
	target->in_combat = true;
	target->combat_rounds = 0;
	target->dir = MOB_DIR_COMBAT_LEFT;

	return true;
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

			if (!player_wilderness_check(p, target)) {
				p->mob.target_player = -1;
				return;
			}

			if (p->projectile != NULL) {
				player_shoot_pvp(p, p->projectile, target);
				return;
			}

			if (p->mob.server->tick_counter <
			    (target->mob.combat_timer + 6)) {
				/*
				 * hp bar in client takes roughly 4 seconds
				 * to be gone
				 */
				p->mob.walk_queue_pos = 0;
				p->mob.walk_queue_len = 0;
				mob_combat_reset(&p->mob);
				return;
			}

			if (!player_init_combat(p, &target->mob)) {
				/* prefer the route from the client if available */
				if ((p->mob.walk_queue_len -
					    p->mob.walk_queue_pos) == 0 &&
				    !mob_within_range(&p->mob,
				    target->mob.x, target->mob.y, 3)) {
					p->mob.following_player = target->mob.id;
					p->mob.following_npc = -1;
				}
				return;
			}

			player_skull(p, target);

			p->mob.target_player = target->mob.id;
			p->mob.target_npc = -1;

			player_close_ui(target);
			player_clear_actions(target);
			target->mob.combat_next_hit = 4;

			player_send_message(target, "You are under attack!");
		} else if (p->mob.target_npc != -1) {
			struct npc *target;

			target = p->mob.server->npcs[p->mob.target_npc];
			if (target == NULL) {
				mob_combat_reset(&p->mob);
				return;
			}

			if (p->projectile != NULL) {
				player_shoot_pvm(p, p->projectile, target);
				return;
			}

			if (!player_init_combat(p, &target->mob)) {
				/* prefer the route from the client if available */
				if ((p->mob.walk_queue_len -
					    p->mob.walk_queue_pos) == 0 &&
				    !mob_within_range(&p->mob,
				    target->mob.x, target->mob.y, 3)) {
					p->mob.following_player = -1;
					p->mob.following_npc = target->mob.id;
				}
				return;
			}

			target->mob.combat_next_hit = 3;
			p->mob.target_player = -1;

			script_onattacknpc(p->mob.server->lua, p, target);
		}
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

		if (p->mob.combat_next_hit > 0) {
			p->mob.combat_next_hit--;
			return;
		}

		roll = player_pvp_roll(p, target);
		player_damage(target, p, roll);
		target->mob.combat_timer = p->mob.server->tick_counter;
		p->mob.combat_rounds++;
		p->mob.combat_next_hit = 3;
	} else if (p->mob.target_npc != -1) {
		struct npc *target;
		int roll;

		target = p->mob.server->npcs[p->mob.target_npc];
		if (target == NULL) {
			mob_combat_reset(&p->mob);
			return;
		}

		if (p->mob.x != target->mob.x ||
		    p->mob.y != target->mob.y) {
			return;
		}

		if (p->mob.dir != MOB_DIR_COMBAT_RIGHT &&
		    p->mob.dir != MOB_DIR_COMBAT_LEFT) {
			p->mob.dir = MOB_DIR_COMBAT_RIGHT;
			target->mob.dir = MOB_DIR_COMBAT_LEFT;
		}

		if (p->mob.combat_next_hit > 0) {
			p->mob.combat_next_hit--;
			return;
		}

		roll = player_pvm_roll(p, target);
		npc_damage(target, p, roll);
		target->mob.combat_timer = p->mob.server->tick_counter;
		p->mob.combat_rounds++;
		p->mob.combat_next_hit = 3;
	}
}

int
player_retreat(struct player *p)
{
	struct player *p2;
	struct npc *npc;

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
			p2->mob.walk_queue_len = 0;
			p2->mob.walk_queue_pos = 0;
			mob_combat_reset(&p2->mob);
		}
	}

	if (p->mob.target_npc != -1) {
		npc = p->mob.server->npcs[p->mob.target_npc];
		if (npc != NULL) {
			npc->mob.walk_queue_len = 0;
			npc->mob.walk_queue_pos = 0;
			mob_combat_reset(&npc->mob);
		}
	}

	mob_combat_reset(&p->mob);
	return 0;
}

void
player_recalculate_equip(struct player *p)
{
	struct item_config *item;
	struct projectile_config *proj;
	uint8_t sprites_orig[MAX_ENTITY_SPRITES];
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

	memcpy(sprites_orig, p->sprites, sizeof(sprites_orig));
	memcpy(p->sprites, p->sprites_base, sizeof(p->sprites));

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

	if (p->bonus_weaponaim != orig_aim ||
	    p->bonus_weaponpower != orig_power ||
	    p->bonus_armour != orig_armour ||
	    p->bonus_magic != orig_magic ||
	    p->bonus_prayer != orig_prayer) {
		p->bonus_changed = true;
	}

	if (memcmp(sprites_orig, p->sprites, sizeof(sprites_orig)) != 0) {
		p->appearance_changed = true;
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
	if (!script_onwearobj(p->mob.server->lua, p, type)) {
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
	player_recalculate_equip(p);
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
	player_recalculate_equip(p);
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

	if (level != p->combat_level) {
		p->combat_level = level;
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

static void
player_stat_changed(struct player *p, int stat)
{
	char mes[64];

	if (stat != SKILL_PRAYER && stat != SKILL_HITS &&
	    p->mob.cur_stats[stat] == p->mob.base_stats[stat]) {
		(void)snprintf(mes, sizeof(mes),
		    "Your %s ability has returned to normal",
		    skill_names[stat]);
		player_send_message(p, mes);
	}
	player_send_stat(p, stat);
}

static void
player_restore_stat(struct player *p, int stat)
{
	if (p->mob.cur_stats[stat] < p->mob.base_stats[stat]) {
		p->mob.cur_stats[stat]++;
		player_stat_changed(p, stat);
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
			player_stat_changed(p, i);
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
	if (config == NULL) {
		return;
	}
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
		if (config == NULL) {
			return;
		}
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
	/* XXX: slow, known_loc_count gets huge */
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
	p->mob.following_player = -1;
	p->mob.following_npc = -1;
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

	for (int x = -2; x < 3; ++x) {
		for (int y = -2; y < 3; ++y) {
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
		if (zone == NULL || zone->plane != orig->plane) {
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
player_takeobject(struct player *p, struct ground_item *item)
{
	struct item_config *config;

	config = server_item_config_by_id(item->id);
	assert(config != NULL);

	player_inv_give(p, config, item->stack);

	if (item->respawn) {
		item->respawn_time = p->mob.server->tick_counter +
		    (config->respawn_rate / 5);
	} else {
		server_remove_temp_item(item->unique_id);
	}
}

void
player_process_action(struct player *p)
{
	struct npc *npc;
	struct player *target;
	struct item_config *item_config;
	struct item_config *item_config2;
	struct ground_item *item;
	struct bound *bound;
	struct loc *loc;
	uint16_t id, id2;
	uint32_t stack;

	if (p->action != ACTION_NONE &&
	    p->action != ACTION_NPC_ATTACK &&
	    p->action != ACTION_NPC_CAST &&
	    p->action != ACTION_PLAYER_ATTACK &&
	    p->action != ACTION_PLAYER_CAST &&
	    /* https://youtu.be/F7vPM8LA1HQ?t=218 */
	    p->action != ACTION_SELF_CAST &&
	    p->mob.in_combat) {
		player_send_message(p,
		    "You can't do that whilst you are fighting");
		p->action = ACTION_NONE;
		return;
	}

	switch (p->action) {
	case ACTION_NPC_ATTACK:
		npc = p->mob.server->npcs[p->action_npc];
		if (npc == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		p->mob.target_npc = p->action_npc;
		p->action = ACTION_NONE;
		break;
	case ACTION_NPC_USEWITH:
		npc = p->mob.server->npcs[p->action_npc];
		if (npc == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		if (p->action_slot >= p->inv_count) {
			p->action = ACTION_NONE;
			return;
		}
		if (!mob_within_range(&p->mob, npc->mob.x, npc->mob.y, 2)) {
			/* let client take first (pathed) steps */
			if ((p->mob.walk_queue_len - p->mob.walk_queue_pos) == 0) {
				p->mob.following_npc = p->action_npc;
				p->mob.following_player = -1;
			}
			return;
		}
		if (!mob_check_reachable(&p->mob, npc->mob.x, npc->mob.y, false)) {
			return;
		}
		p->mob.walk_queue_len = 0;
		p->mob.walk_queue_pos = 0;
		id = p->inventory[p->action_slot].id;
		item_config = server_item_config_by_id(id);
		assert(item_config != NULL);
		script_onusenpc(p->mob.server->lua, p, npc, item_config);
		p->action = ACTION_NONE;
		break;
	case ACTION_NPC_TALK:
		npc = p->mob.server->npcs[p->action_npc];
		if (npc == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		if (!mob_within_range(&p->mob, npc->mob.x, npc->mob.y, 2)) {
			/* let client take first (pathed) steps */
			if ((p->mob.walk_queue_len - p->mob.walk_queue_pos) == 0) {
				p->mob.following_npc = p->action_npc;
				p->mob.following_player = -1;
			}
			return;
		}
		p->action = ACTION_NONE;
		p->mob.walk_queue_len = 0;
		p->mob.walk_queue_pos = 0;
		if (!mob_check_reachable(&p->mob, npc->mob.x, npc->mob.y, false)) {
			return;
		}
		if (npc->busy) {
			char mes[128];

			(void)snprintf(mes, sizeof(mes),
			    "%s is busy at the moment", npc->config->names[0]);
			player_send_message(p, mes);
		} else {
			npc->talk_target = p->mob.id;
			mob_face(&p->mob, npc->mob.x, npc->mob.y);
			script_onnpctalk(p->mob.server->lua, p, npc);
		}
		break;
	case ACTION_INV_DROP:
		if (p->action_slot >= p->inv_count) {
			p->action = ACTION_NONE;
			return;
		}

		if (p->mob.walk_queue_len > 0 || p->ui_trade_open) {
			return;
		}

		id = p->inventory[p->action_slot].id;
		stack = p->inventory[p->action_slot].stack;
		item_config = server_item_config_by_id(id);
		assert(item_config != NULL);
		player_inv_remove(p, item_config, stack);
		server_add_temp_item(p, p->mob.x, p->mob.y, id, stack);
		p->action = ACTION_NONE;
		break;
	case ACTION_INV_OP1:
		if (p->action_slot >= p->inv_count) {
			p->action = ACTION_NONE;
			return;
		}
		id = p->inventory[p->action_slot].id;
		item_config = server_item_config_by_id(id);
		script_onopinv(p->mob.server->lua, p, item_config);
		p->action = ACTION_NONE;
		break;
	case ACTION_INV_USEWITH:
		if (p->action_slot >= p->inv_count ||
		    p->action_slot2 >= p->inv_count) {
			p->action = ACTION_NONE;
			return;
		}
		id = p->inventory[p->action_slot].id;
		id2 = p->inventory[p->action_slot2].id;
		item_config = server_item_config_by_id(id);
		item_config2 = server_item_config_by_id(id2);
		if (item_config == NULL || item_config2 == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		script_onuseinv(p->mob.server->lua, p,
		    item_config, item_config2);
		p->action = ACTION_NONE;
		break;
	case ACTION_INV_CAST:
		if (p->action_slot >= p->inv_count) {
			p->action = ACTION_NONE;
			return;
		}
		id = p->inventory[p->action_slot].id;
		item_config = server_item_config_by_id(id);
		if (p->spell == NULL || item_config == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		script_onspellinv(p->mob.server->lua, p, item_config);
		p->action = ACTION_NONE;
		break;
	case ACTION_ITEM_TAKE:
		if (p->action_item == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		item = server_find_ground_item(p,
		    p->action_item->x, p->action_item->y,
		    p->action_item->id);
		item_config = server_item_config_by_id(p->action_item->id);
		if (item == NULL || item_config == NULL ||
		    p->inv_count >= MAX_INV_SIZE ||
		    !player_can_see_item(p, item)) {
			p->action = ACTION_NONE;
			return;
		}
		if (!mob_reached_item(&p->mob, item)) {
			return;
		}
		if (!script_ontakeobj(p->mob.server->lua, p, item)) {
			player_takeobject(p, item);
		}
		p->action = ACTION_NONE;
		break;
	case ACTION_ITEM_USEWITH:
		item = server_find_ground_item(p,
		    p->action_item->x, p->action_item->y,
		    p->action_item->id);
		if (p->action_slot >= p->inv_count || item == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		if (!mob_reached_item(&p->mob, item)) {
			return;
		}
		id = p->inventory[p->action_slot].id;
		item_config = server_item_config_by_id(item->id);
		item_config2 = server_item_config_by_id(id);
		if (item_config == NULL || item_config2 == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		script_onuseobj(p->mob.server->lua, p,
		    item_config, p->action_item->x, p->action_item->y,
		    item_config2);
		p->action = ACTION_NONE;
		break;
	case ACTION_NPC_CAST:
		npc = p->mob.server->npcs[p->action_npc];
		if (npc == NULL) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		/* should be able to shoot within 4 tiles */
		if (!mob_within_range(&p->mob,
		    npc->mob.x, npc->mob.y, 4)) {
			return;
		}
		p->mob.walk_queue_len = 0;
		p->mob.walk_queue_pos = 0;
		p->action = ACTION_NONE;
		if (npc->config->aggression == 0) {
			player_send_message(p, "I can't attack that");
			return;
		}
		if (!player_can_cast(p, p->spell)) {
			return;
		}
		/*
		 * line of sight check should be AFTER spell timer is set
		 * see Logg/Tylerbeg/08-05-2018 20.12.26 for some reason, I go to the wizards tower, cast fire blast on the demon for a little while, forget what Im doing and leave
		 */
		script_onskillnpc(p->mob.server->lua, p, npc, p->spell);
		break;
	case ACTION_PLAYER_ATTACK:
		target = p->mob.server->players[p->action_player];
		if (target == NULL) {
			p->action = ACTION_NONE;
			return;
		}
		p->mob.target_player = target->mob.id;
		p->action = ACTION_NONE;
		break;
	case ACTION_PLAYER_CAST:
		target = p->mob.server->players[p->action_player];
		if (target == NULL) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		/* should be able to shoot within 4 tiles */
		if (!mob_within_range(&p->mob,
		    target->mob.x, target->mob.y, 4)) {
			return;
		}
		p->mob.walk_queue_len = 0;
		p->mob.walk_queue_pos = 0;
		p->action = ACTION_NONE;
		if (!player_wilderness_check(p, target) ||
		    !player_can_cast(p, p->spell)) {
			return;
		}
		/*
		 * line of sight check should be AFTER spell timer is set
		 * see Logg/Tylerbeg/08-05-2018 20.12.26 for some reason, I go to the wizards tower, cast fire blast on the demon for a little while, forget what Im doing and leave
		 */
		script_onskillplayer(p->mob.server->lua, p, target, p->spell);
		break;
	case ACTION_BOUND_OP1:
	case ACTION_BOUND_OP2:
		if (p->action_bound == NULL) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		bound = server_find_bound(p->action_bound->x, p->action_bound->y,
		    p->action_bound->dir);
		if (bound == NULL) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		if (!mob_reached_bound(&p->mob, bound)) {
			return;
		}
		p->mob.walk_queue_len = 0;
		p->mob.walk_queue_pos = 0;
		if (p->action == ACTION_BOUND_OP1) {
			script_onopbound1(p->mob.server->lua, p, bound);
		} else {
			script_onopbound2(p->mob.server->lua, p, bound);
		}
		p->action = ACTION_NONE;
		break;
	case ACTION_BOUND_USEWITH:
		if (p->action_bound == NULL || p->action_slot >= p->inv_count) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		bound = server_find_bound(p->action_bound->x, p->action_bound->y,
		    p->action_bound->dir);
		if (bound == NULL) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		if (!mob_reached_bound(&p->mob, bound)) {
			return;
		}
		item_config = server_item_config_by_id(p->inventory[p->action_slot].id);
		assert(item_config != NULL);
		p->action = ACTION_NONE;
		script_onusebound(p->mob.server->lua, p, bound, item_config);
		break;
	case ACTION_LOC_USEWITH:
		if (p->action_loc == NULL || p->action_slot >= p->inv_count) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		loc = server_find_loc(p->action_loc->x, p->action_loc->y);
		if (loc == NULL) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		if (!mob_reached_loc(&p->mob, loc) ||
		    (p->mob.walk_queue_len - p->mob.walk_queue_pos) > 0) {
			return;
		}
		item_config = server_item_config_by_id(p->inventory[p->action_slot].id);
		assert(item_config != NULL);
		p->action = ACTION_NONE;
		script_onuseloc(p->mob.server->lua, p, loc, item_config);
		break;
	case ACTION_LOC_OP1:
	case ACTION_LOC_OP2:
		if (p->action_loc == NULL) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		loc = server_find_loc(p->action_loc->x, p->action_loc->y);
		if (loc == NULL) {
			p->action = ACTION_NONE;
			p->mob.walk_queue_len = 0;
			p->mob.walk_queue_pos = 0;
			return;
		}
		if (!mob_reached_loc(&p->mob, loc) ||
		    (p->mob.walk_queue_len - p->mob.walk_queue_pos) > 0) {
			return;
		}
		if (p->action == ACTION_LOC_OP1) {
			script_onoploc1(p->mob.server->lua, p, loc);
		} else {
			script_onoploc2(p->mob.server->lua, p, loc);
		}
		p->action = ACTION_NONE;
		break;
	case ACTION_SELF_CAST:
		p->action = ACTION_NONE;
		if (!player_can_cast(p, p->spell)) {
			return;
		}
		script_onspellself(p->mob.server->lua, p, p->spell);
		break;
	}
}

bool
player_has_reagents(struct player *p, struct spell_config *spell)
{
	struct item_config *config;
	struct item_config *staff;

	if (p->mob.server->cast_without_runes) {
		return true;
	}
	for (int i = 0; i < spell->reagent_count; ++i) {
		switch (spell->reagents[i].item_id) {
		case ITEM_FIRE_RUNE:
			staff = server_find_item_config("staff of fire");
			break;
		case ITEM_WATER_RUNE:
			staff = server_find_item_config("staff of water");
			break;
		case ITEM_AIR_RUNE:
			staff = server_find_item_config("staff of air");
			break;
		case ITEM_EARTH_RUNE:
			staff = server_find_item_config("staff of earth");
			break;
		default:
			staff = NULL;
			break;
		}
		if (staff != NULL && player_inv_worn(p, staff)) {
			continue;
		}
		config = server_item_config_by_id(spell->reagents[i].item_id);
		assert(config != NULL);
		if (!player_inv_held(p, config, spell->reagents[i].amount)) {
			return false;
		}
	}
	return true;
}

bool
player_can_cast(struct player *p, struct spell_config *spell)
{
	int magic_level;

	assert(spell != NULL);

	if (spell->level > p->mob.cur_stats[SKILL_MAGIC] ||
	    !player_has_reagents(p, spell)) {
		/*
		 * normally the client validates this and produces
		 * a nice message
		 */
		return false;
	}
	if (p->mob.server->spell_timer && p->spell_timer > 0) {
		char mes[64];

		(void)snprintf(mes, sizeof(mes),
		    "You need to wait %d seconds before you can cast another spell",
		    (p->spell_timer + 1) / 2);
		player_send_message(p, mes);
		return false;
	}

	/*
	 * XXX unlikely we'll ever have enough data for this to be accurate
	 * https://classic.runescape.wiki/w/User:Stormykins/Magic_research
	 */
	magic_level = p->mob.cur_stats[SKILL_MAGIC];
	if ((magic_level - spell->level) < 10) {
		double r1 = server_random();
		double r2 = server_random();
		double roll_spell = spell->level * r1;
		double roll_player = (magic_level + (p->bonus_magic * 1.5)) * r2;
		if (roll_player < roll_spell) {
			player_send_message(p,
			    "The spell fails! You may try again in 20 seconds");
			p->spell_timer = 40;
			return false;
		}
	}
	p->spell_timer = 3;
	return true;
}

void
player_init_class(struct player *p)
{
	struct item_config *item;

	/* XXX the experience rewards here are pretty much guessed */

	switch (p->rpg_class) {
	case CLASS_ADVENTURER:
		item = server_find_item_config("bronze axe");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		item = server_find_item_config("tinderbox");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		item = server_find_item_config("jug");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		item = server_find_item_config("pot");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		stat_advance(p, SKILL_ATTACK, 500, 0);
		stat_advance(p, SKILL_DEFENSE, 500, 0);
		stat_advance(p, SKILL_STRENGTH, 500, 0);
		stat_advance(p, SKILL_HITS, 1500, 0);
		stat_advance(p, SKILL_RANGED, 500, 0);
		stat_advance(p, SKILL_MAGIC, 500, 0);
		stat_advance(p, SKILL_PRAYER, 500, 0);
		break;
	case CLASS_WARRIOR:
		item = server_find_item_config("bronze short sword");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		item = server_find_item_config("wooden shield");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		stat_advance(p, SKILL_ATTACK, 700, 0);
		stat_advance(p, SKILL_DEFENSE, 700, 0);
		stat_advance(p, SKILL_STRENGTH, 700, 0);
		stat_advance(p, SKILL_HITS, 3000, 0);
		break;
	case CLASS_WIZARD:
		item = server_find_item_config("staff");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		item = server_find_item_config("bluewizhat");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		stat_advance(p, SKILL_MAGIC, 2600, 0);
		break;
	case CLASS_RANGER:
		item = server_find_item_config("shortbow");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}

		item = server_find_item_config("arrows");
		if (item != NULL) {
			player_inv_give(p, item, 10);
		}

		stat_advance(p, SKILL_RANGED, 2100, 0);
		stat_advance(p, SKILL_HITS, 3000, 0);
		break;
	case CLASS_MINER:
		item = server_find_item_config("pickaxe");
		if (item != NULL) {
			player_inv_give(p, item, 1);
		}
		stat_advance(p, SKILL_MINING, 2600, 0);
		break;
	}

	player_send_init_stats(p);
}

void
player_init_adventurer(struct player *p)
{
	struct item_config *item;

	/* post-tutorial island starter pack */

	item = server_find_item_config("bronze axe");
	if (item != NULL) {
		player_inv_give(p, item, 1);
	}

	item = server_find_item_config("tinderbox");
	if (item != NULL) {
		player_inv_give(p, item, 1);
	}

	item = server_find_item_config("cookedmeat");
	if (item != NULL) {
		player_inv_give(p, item, 1);
	}
}

int32_t
player_variable_get(struct player *p, const char *varname)
{
	for (size_t i = 0; i < p->variable_count; i++) {
		if (strcasecmp(varname, p->variables[i].name) == 0) {
			return p->variables[i].value;
		}
	}
	return 0;
}

void
player_variable_set(struct player *p, const char *varname, int32_t value)
{
	/* check for an existing variable and update/delete it */
	for (size_t i = 0; i < p->variable_count; i++) {
		if (strcasecmp(varname, p->variables[i].name) == 0) {
			/* remove the variable if value is 0 */
			if (value == 0) {
				free(p->variables[i].name);

				/* shift all elements to the left */
				for (size_t j = i + 1; j < p->variable_count; j++) {
					p->variables[j - 1] = p->variables[j];
				}

				p->variable_count -= 1;
			} else {
				p->variables[i].value = value;
			}
			return;
		}
	}

	/* create a new variable */
	if (p->variable_count >= p->variable_max) {
		size_t next_size = p->variable_max + PLAYER_VAR_INC_SIZE;

		if (reallocarr(&p->variables, next_size,
			sizeof(struct playervar)) == -1) {
			printf("failed to realloc player var array of size %zu\n",
				next_size);
			return;
		}

		p->variable_max = next_size;
	}

	p->variables[p->variable_count].name = strdup(varname);
	p->variables[p->variable_count].value = value;
	p->variable_count += 1;
}

void
player_send_welcome(struct player *p)
{
	char mes[32];
	(void)snprintf(mes, sizeof(mes), "Welcome to %s!", p->mob.server->name);
	player_send_message(p, mes);
}

static void
player_moved(struct player *p, int from_x, int from_y)
{
	struct zone *zone_old;
	struct zone *zone_new;

	zone_old = server_find_zone(from_x, from_y);
	zone_new = server_find_zone(p->mob.x, p->mob.y);

	if (zone_old != NULL) {
		if (zone_old != zone_new) {
			zone_remove_player(zone_old, p->mob.id);
		}
	}

	if (zone_new != NULL) {
		if (zone_old != zone_new) {
			zone_add_player(zone_new, p->mob.id);
		}
	}

	if (p->chased_by_npc != UINT16_MAX) {
		struct npc *npc = p->mob.server->npcs[p->chased_by_npc];
		if (npc == NULL) {
			return;
		}
		/* TODO: maybe should check visibility */
		if (!mob_within_range(&npc->mob, p->mob.x, p->mob.y, 16)) {
			p->chased_by_npc = UINT16_MAX;
		}
	}
}

void
player_teleport(struct player *p, int x, int y)
{
	int orig_x = p->mob.x;
	int orig_y = p->mob.y;

	p->mob.x = x;
	p->mob.y = y;

	if (p->mob.target_player != -1) {
		struct player *p2;

		/* no retreat message per https://youtu.be/F7vPM8LA1HQ?t=218 */

		p2 = p->mob.server->players[p->mob.target_player];
		if (p2 != NULL) {
			p2->mob.walk_queue_len = 0;
			p2->mob.walk_queue_pos = 0;
			mob_combat_reset(&p2->mob);
		}
	}

	if (p->mob.target_npc != -1) {
		struct npc *npc;

		npc = p->mob.server->npcs[p->mob.target_npc];
		if (npc != NULL) {
			npc->mob.walk_queue_len = 0;
			npc->mob.walk_queue_pos = 0;
			mob_combat_reset(&npc->mob);
		}
	}

	mob_combat_reset(&p->mob);

	player_moved(p, orig_x, orig_y);

	p->teleported = true;
	player_send_plane_init(p);
}

void
player_process_movement(struct player *p)
{
	int orig_x = p->mob.x;
	int orig_y = p->mob.y;

	mob_process_walk_queue(&p->mob);

	player_moved(p, orig_x, orig_y);
}

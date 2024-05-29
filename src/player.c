#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "config/anim.h"
#include "protocol/opcodes.h"
#include "protocol/utility.h"
#include "server.h"
#include "entity.h"
#include "loop.h"
#include "netio.h"

static int player_pvp_roll(struct player *, struct player *);

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
	p->sprites[ANIM_SLOT_HEAD] = ANIM_HEAD1 + 1;
	p->sprites[ANIM_SLOT_BODY] = ANIM_BODY1 + 1;
	p->sprites[ANIM_SLOT_LEGS] = ANIM_LEGS1 + 1;
	p->hair_colour = COLOUR_HAIR_DEFAULT;
	p->top_colour = COLOUR_TOP_DEFAULT;
	p->leg_colour = COLOUR_LEG_DEFAULT;
	p->combat_level = 3;
	p->weapon_aim = 1;
	p->weapon_power = 1;
	p->armour = 1;
	p->stats_changed = true;
	p->appearance_changed = true;
	p->plane_changed = true;
	p->ui_design_open = true;
	p->following_player = -1;
	p->last_packet = s->tick_counter;

	p->mob.server = s;
	/*p->mob.x = 120;
	p->mob.y = 648;*/
	p->mob.x = 333;
	p->mob.y = 333;
	p->mob.damage = UINT8_MAX;
	mob_combat_reset(&p->mob);
	s->players[slot] = p;

	loop_add_player(p);
	return p;
}

void
player_process_walk_queue(struct player *p)
{
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
	p->ui_dialog_open = false;
	p->ui_design_open = false;
	p->ui_bank_open = false;
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
player_pvp_roll(struct player *attacker, struct player *defender)
{
	int att = attacker->mob.cur_stats[SKILL_ATTACK];
	int def = defender->mob.cur_stats[SKILL_DEFENSE];
	int str = attacker->mob.cur_stats[SKILL_STRENGTH];

	switch (attacker->combat_style) {
	case COMBAT_STYLE_CONTROLLED:
		att++;
		str++;
		break;
	case COMBAT_STYLE_AGGRESSIVE:
		str += 3;
		break;
	case COMBAT_STYLE_ACCURATE:
		att += 3;
		break;
	}

	switch (defender->combat_style) {
	case COMBAT_STYLE_CONTROLLED:
		def++;
		break;
	case COMBAT_STYLE_DEFENSIVE:
		def += 3;
		break;
	}

	return mob_combat_roll(&attacker->mob.server->ran,
	    att, attacker->weapon_aim,
	    def, defender->armour,
	    str, attacker->weapon_power);
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
player_die(struct player *p)
{
	/* TODO teleportation */
	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		p->mob.cur_stats[i] = p->mob.base_stats[i];
	}
	p->following_player = -1;
	p->walk_queue_len = 0;
	p->walk_queue_pos = 0;
	mob_combat_reset(&p->mob);
	player_send_death(p);
}

void
player_process_combat(struct player *p)
{
	if (!p->mob.in_combat) {
		if (p->mob.target_player != -1) {
			struct player *target;

			/* TODO: check whether in wilderness */

			if (p->mob.server->tick_counter <
			    (p->mob.combat_timer + 3)) {
				p->walk_queue_pos = 0;
				p->walk_queue_len = 0;
				mob_combat_reset(&p->mob);
				return;
			}

			target = p->mob.server->players[p->mob.target_player];
			if (target == NULL) {
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

			if (mob_within_range(&p->mob,
			    target->mob.x, target->mob.y, 3)) {
				target->walk_queue_len = 0;
				target->walk_queue_pos= 0;
			}

			if (p->mob.x != target->mob.x ||
			    p->mob.y != target->mob.y) {
				return;
			}

			p->following_player = -1;
			p->walk_queue_len = 0;
			p->walk_queue_pos = 0;
			p->mob.target_player = target->mob.id;
			p->mob.target_npc = -1;
			p->mob.in_combat = true;
			p->mob.combat_next_hit = 0;
			p->mob.combat_rounds = 0;
			p->mob.dir = MOB_DIR_COMBAT_RIGHT;

			target->following_player = -1;
			target->walk_queue_len = 0;
			target->walk_queue_pos = 0;
			target->mob.target_player = p->mob.id;
			target->mob.target_npc = -1;
			target->mob.in_combat = true;
			target->mob.combat_next_hit =
			    target->mob.server->tick_counter + 3;
			target->mob.combat_rounds = 0;
			target->mob.dir = MOB_DIR_COMBAT_LEFT;

			player_close_ui(target);
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
		roll = player_pvp_roll(p, target);
		if (roll >= target->mob.cur_stats[SKILL_HITS]) {
			char name[32], msg[64];

			mob_combat_reset(&p->mob);
			player_die(target);
			mod37_namedec(target->name, name);
			/* TODO give experience */
			(void)snprintf(msg, sizeof(msg),
			    "You have defeated %s!", name);
			player_send_message(p, msg);
			return;
		}
		target->mob.cur_stats[SKILL_HITS] -= roll;
		target->mob.damage = roll;
		target->mob.combat_rounds++;
	}

	p->mob.combat_next_hit = p->mob.server->tick_counter + 4;
	p->mob.combat_timer = p->mob.server->tick_counter;
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

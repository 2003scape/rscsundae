#include <assert.h>
#include "config/config.h"
#include "entity.h"
#include "script.h"
#include "server.h"

static void npc_random_walk(struct npc *);
static int npc_combat_roll(struct npc *, struct player *);
static void npc_hunt_target(struct npc *);
static bool npc_init_combat(struct npc *, struct player *);

void
npc_die(struct npc *npc, struct player *p)
{
	if (p != NULL) {
		if (!script_onkillnpc(npc->mob.server->lua, p, npc)) {
			for (int i = 0; i < npc->config->drop_count; ++i) {
				struct item_config *item_config;
				uint16_t id;

				id = npc->config->drops[i].id;
				item_config = server_item_config_by_id(id);
				assert(item_config != NULL);
				server_add_temp_item(p, npc->mob.x, npc->mob.y,
				    id, npc->config->drops[i].amount);
			}
		} else {
			script_process(p->mob.server->lua, p);
			if (npc->mob.cur_stats[SKILL_HITS] > 0) {
				/* the killnpc trigger can restore hits */
				return;
			}
		}
	}

	npc->respawn_time = npc->config->respawn / 5;

	if (p != NULL && p->mob.in_combat &&
	    p->mob.target_npc == npc->mob.id) {
		player_award_combat_xp(p, &npc->mob);
	}

	mob_die(&npc->mob);

	if (p->mob.target_npc == npc->mob.id) {
		mob_combat_reset(&p->mob);
	}

	npc->mob.x = npc->spawn_x;
	npc->mob.y = npc->spawn_y;
}

void
npc_damage(struct npc *npc, struct player *p , int roll)
{
	if (roll >= npc->mob.cur_stats[SKILL_HITS]) {
		npc->mob.cur_stats[SKILL_HITS] = 0;
		npc_die(npc, p);
	}

	if (npc->mob.cur_stats[SKILL_HITS] == 0) {
		return;
	}

	npc->mob.cur_stats[SKILL_HITS] -= roll;
	npc->mob.damage = roll;
	npc->mob.damage_timer = p->mob.server->tick_counter;
}

static void
npc_random_walk(struct npc *npc)
{
	double rx, ry;
	int x, y;
	int min_x, min_y;
	bool valid;
	int attempts;

	if (npc->config->wander_range == 0) {
		return;
	}

	if (npc->mob.in_combat && npc->mob.target_player != -1) {
		struct player *p;

		p = npc->mob.server->players[npc->mob.target_player];
		if (p != NULL) {
			player_send_message(p,
			    "Your opponent is retreating!");
			mob_combat_reset(&npc->mob);
			mob_combat_reset(&p->mob);
		}
	}

	attempts = 0;

	do {
		rx = server_random();
		ry = server_random();

		min_x = npc->spawn_x - npc->config->wander_range;
		if (min_x < 0) {
			min_x = 0;
		}
		x = min_x + (int)((npc->config->wander_range * 2) * rx);

		min_y = npc->spawn_y - npc->config->wander_range;
		if (min_y < 0) {
			min_y = 0;
		}
		y = min_y + (int)((npc->config->wander_range * 2) * ry);

		valid = true;

		if (npc->mob.y > PLANE_LEVEL_INC) {
			break;
		}

		switch (npc->config->move_restrict) {
		case MOVE_RESTRICT_OUTDOORS:
			valid = (npc->mob.server->roofs[x][y] == 0);
			break;
		case MOVE_RESTRICT_INDOORS:
			valid = (npc->mob.server->roofs[x][y] != 0);
			break;
		}
	} while (!valid && (attempts++) < 10);

	npc->mob.walk_queue_x[0] = x;
	npc->mob.walk_queue_y[0] = y;
	npc->mob.walk_queue_len = 1;
	npc->mob.walk_queue_pos = 0;
}

static void
npc_hunt_target(struct npc *npc)
{
	struct player *p;
	struct player *players[128];
	bool restrict_hunt = false;
	size_t n;

	if (npc->mob.cur_stats[SKILL_HITS] <= npc->config->bravery) {
		return;
	}

	if (npc->mob.server->restrict_npc_aggression) {
		restrict_hunt = true;
		if (mob_wilderness_level(&npc->mob) > 0) {
			restrict_hunt = false;
		}
	}

	n = mob_get_nearby_players(&npc->mob, players, 128);
	for (size_t i = 0; i < n; ++i) {
		p = players[i];

		if (p->mob.in_combat || p->chased_by_npc != UINT16_MAX) {
			continue;
		}

		if (restrict_hunt &&
		    p->combat_level > (npc->config->combat_level * 2)) {
			continue;
		}

		if (!mob_within_range(&p->mob, npc->mob.x, npc->mob.y,
		    npc->config->hunt_range + 2)) {
			continue;
		}

		p->chased_by_npc = npc->mob.id;
		npc->mob.following_player = p->mob.id;
		npc->mob.target_player = p->mob.id;
		return;
	}
}

void
npc_process_movement(struct npc *npc)
{
	struct zone *zone_old;
	struct zone *zone_new;

	if (npc->busy) {
		return;
	}

	/*
	 * like players, NPCs appear to be stunned slightly after combat, see
	 * RSC 2001/replays master archive/Walk around/Misthalin- Lumbridge/walkaround- lumbridge road to varrock- road up to wheatfield digsite- dark mage aggressive - lvl 1-1-1
	 */
	if (npc->mob.server->tick_counter < (npc->mob.combat_timer + 6)) {
		return;
	}

	zone_old = server_find_zone(npc->mob.x, npc->mob.y);

	if (npc->mob.in_combat) {
		struct player *p;

		p = npc->mob.server->players[npc->mob.target_player];
		if (npc->mob.x != p->mob.x || npc->mob.y != p->mob.y) {
			npc->mob.walk_queue_x[0] = p->mob.x;
			npc->mob.walk_queue_y[0] = p->mob.y;
			npc->mob.walk_queue_len = 1;
			npc->mob.walk_queue_pos = 0;
		}
	} else {
		if (npc->mob.following_player != -1) {
			struct player *p;

			p = npc->mob.server->players[npc->mob.following_player];
			if (p == NULL) {
				npc->mob.following_player = -1;
				return;
			}

			/*
			 * aggressive NPCs used to be able to get stuck outside
			 * of their range, see various replays of jungle
			 * spiders on hazelmere's island
			 *
			 * TODO maybe should check visibility too
			 */
			if (!mob_within_range(&p->mob,
			    npc->spawn_x, npc->spawn_y,
			    npc->config->wander_range * 2)) {
				p->chased_by_npc = UINT16_MAX;
				npc->mob.following_player = -1;
				npc->mob.target_player = -1;
				return;
			}
		} else if (npc->config->aggression > 2) {
			npc_hunt_target(npc);
		}

		if (npc->random_walk_timer == 0) {
			if (npc->mob.following_player == -1) {
				double r = ranval(&npc->mob.server->ran) /
				    (double)UINT32_MAX;

				/* XXX: needs verifying */
				npc->random_walk_timer = 1 + (int)(r * 30);
				npc_random_walk(npc);
			}
		} else {
			npc->random_walk_timer--;
		}
	}

	int pos = npc->mob.walk_queue_pos;
	if ((npc->mob.walk_queue_len - pos) <= 0) {
		return;
	}
	/* TODO: also need to stop them walking on same tiles as other NPCs */
	int x = npc->mob.walk_queue_x[pos];
	int y = npc->mob.walk_queue_y[pos];
	switch (npc->config->move_restrict) {
	case MOVE_RESTRICT_OUTDOORS:
		if (y < PLANE_LEVEL_INC && npc->mob.server->roofs[x][y] != 0) {
			npc->mob.walk_queue_len = 0;
			npc->mob.walk_queue_pos = 0;
		}
		break;
	case MOVE_RESTRICT_INDOORS:
		if (y < PLANE_LEVEL_INC && npc->mob.server->roofs[x][y] == 0) {
			npc->mob.walk_queue_len = 0;
			npc->mob.walk_queue_pos = 0;
		}
		break;
	}

	mob_process_walk_queue(&npc->mob);

	zone_new = server_find_zone(npc->mob.x, npc->mob.y);

	if (zone_old != zone_new) {
		if (zone_old != NULL) {
			zone_remove_npc(zone_old, npc->mob.id);
		}
		if (zone_new != NULL) {
			zone_add_npc(zone_new, npc->mob.id);
		}
	}
}

static int
npc_combat_roll(struct npc *npc, struct player *defender)
{
	int def = player_get_defense_boosted(defender);

	return mob_combat_roll(npc->mob.cur_stats[SKILL_ATTACK], 0,
	    def, defender->bonus_armour,
	    npc->mob.cur_stats[SKILL_STRENGTH], 0);
}

static bool
npc_init_combat(struct npc *npc, struct player *target)
{
	/*
	 * like players, NPCs appear to be stunned slightly after combat, see
	 * RSC 2001/replays master archive/Walk around/Misthalin- Lumbridge/walkaround- lumbridge road to varrock- road up to wheatfield digsite- dark mage aggressive - lvl 1-1-1
	 */

	if (target->mob.in_combat ||
	    npc->mob.server->tick_counter <
	    (target->mob.combat_timer + 6)) {
		target->chased_by_npc = UINT16_MAX;
		npc->mob.walk_queue_pos = 0;
		npc->mob.walk_queue_len = 0;
		mob_combat_reset(&npc->mob);
		return false;
	}

	if (!mob_within_range(&npc->mob, target->mob.x, target->mob.y, 2)) {
		npc->mob.following_player = target->mob.id;
		return false;
	}

	player_send_message(target, "You are under attack!");

	npc->mob.walk_queue_len = 0;
	npc->mob.walk_queue_pos = 0;

	/* successful catch, combat lock the target */
	target->mob.walk_queue_len = 0;
	target->mob.walk_queue_pos = 0;
	target->mob.following_player = -1;
	target->mob.following_npc = -1;

	npc->mob.following_player = -1;
	npc->mob.following_npc = -1;

	player_close_ui(target);
	player_clear_actions(target);

	npc->mob.in_combat = true;
	npc->mob.combat_next_hit = 0;
	npc->mob.combat_rounds = 0;

	target->mob.target_player = -1;
	target->mob.target_npc = npc->mob.id;
	target->mob.in_combat = true;
	target->mob.combat_rounds = 0;
	target->mob.dir = MOB_DIR_COMBAT_RIGHT;
	return true;
}

void
npc_process_combat(struct npc *npc)
{
	struct player *target;
	int roll;

	if (npc->mob.target_player == -1) {
		return;
	}

	target = npc->mob.server->players[npc->mob.target_player];

	if (target == NULL) {
		mob_combat_reset(&npc->mob);
		return;
	}

	if (!npc->mob.in_combat) {
		npc_init_combat(npc, target);
		return;
	}

	target->chased_by_npc = UINT16_MAX;
	npc->mob.following_player = -1;

	if (target->mob.dir == MOB_DIR_COMBAT_RIGHT) {
		if (npc->mob.x != target->mob.x ||
		    npc->mob.y != target->mob.y) {
			return;
		}
		if (!npc->mob.moved) {
			npc->mob.dir = MOB_DIR_COMBAT_LEFT;
		}
	}

	if (npc->mob.dir != MOB_DIR_COMBAT_LEFT) {
		return;
	}

	/*
	 * check out replay:
	 * rsc-preservation.xyz/Combat/Chickens [Feather Gathering] pt1
	 * for some recordings of retreating from low hits
	 */
	if (npc->mob.combat_rounds >= 3 &&
	    npc->mob.cur_stats[SKILL_HITS] <= npc->config->bravery) {
		npc_random_walk(npc);
		return;
	}

	if (npc->mob.combat_next_hit > 0) {
		npc->mob.combat_next_hit--;
		return;
	}

	roll = npc_combat_roll(npc, target);
	player_damage(target, NULL, roll);
	npc->mob.combat_rounds++;
	npc->mob.combat_next_hit = 3;
	target->mob.combat_timer = npc->mob.server->tick_counter;
}

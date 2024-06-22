#include <assert.h>
#include "config/config.h"
#include "entity.h"
#include "script.h"
#include "server.h"

static void npc_random_walk(struct npc *);
static int npc_combat_roll(struct npc *, struct player *);

void
npc_die(struct npc *npc, struct player *p)
{
	npc->respawn_time = npc->config->respawn / 5;

	if (p != NULL && p->mob.in_combat &&
	    p->mob.target_npc == npc->mob.id) {
		player_award_combat_xp(p, &npc->mob);
	}

	mob_die(&npc->mob);

	if (p->mob.target_npc == npc->mob.id) {
		mob_combat_reset(&p->mob);
        }

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
		}
	}

	npc->mob.x = npc->spawn_x;
	npc->mob.y = npc->spawn_y;
}

void
npc_damage(struct npc *npc, struct player *p , int roll)
{
	if (roll >= npc->mob.cur_stats[SKILL_HITS]) {
		npc_die(npc, p);
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
		rx = ranval(&npc->mob.server->ran) /
		    (double)UINT32_MAX;
		ry = ranval(&npc->mob.server->ran) /
		    (double)UINT32_MAX;

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

void
npc_process_movement(struct npc *npc)
{
	if (npc->busy || npc->mob.in_combat) {
		return;
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
}

static int
npc_combat_roll(struct npc *npc, struct player *defender)
{
	int def = player_get_defense_boosted(defender);

	return mob_combat_roll(&npc->mob.server->ran,
	    npc->mob.cur_stats[SKILL_ATTACK], 0,
	    def, defender->bonus_armour,
	    npc->mob.cur_stats[SKILL_STRENGTH], 0);
}

void
npc_process_combat(struct npc *npc)
{
	if (!npc->mob.in_combat) {
		return;
	}
	if (npc->mob.target_player != -1) {
		struct player *target;
		int roll;

		target = npc->mob.server->players[npc->mob.target_player];
		if (target == NULL) {
			mob_combat_reset(&npc->mob);
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
}

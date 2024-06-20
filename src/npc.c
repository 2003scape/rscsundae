#include <assert.h>
#include "config/config.h"
#include "entity.h"
#include "server.h"

static void npc_random_walk(struct npc *);

void
npc_die(struct npc *npc, struct player *p)
{
	struct item_config *item_config;

	npc->respawn_time = npc->config->respawn / 5;

	mob_die(&npc->mob);

	if (p->mob.target_npc == npc->mob.id) {
		mob_combat_reset(&p->mob);
        }

	for (int i = 0; i < npc->config->drop_count; ++i) {
		item_config = server_item_config_by_id(npc->config->drops[i].id);
		assert(item_config != NULL);
		server_add_temp_item(p, npc->mob.x, npc->mob.y,
		    item_config->id,
		    npc->config->drops[i].amount);
	}
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
	if (npc->random_walk_timer == 0) {
		if (npc->mob.following_player == -1) {
			double r = ranval(&npc->mob.server->ran) /
			    (double)UINT32_MAX;

			/* XXX: needs verifying */
			npc->random_walk_timer = 1 + (int)(r * 40);
			npc_random_walk(npc);
		}
	} else {
		npc->random_walk_timer --;
	}
	mob_process_walk_queue(&npc->mob);
}

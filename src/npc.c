#include <assert.h>
#include "config/config.h"
#include "entity.h"
#include "server.h"

void
npc_die(struct npc *npc, struct player *p)
{
	struct item_config *item_config;

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

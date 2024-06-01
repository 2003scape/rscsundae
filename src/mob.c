#include <math.h>
#include "entity.h"
#include "server.h"

static int mob_combat_max_roll(int, int);
static int mob_combat_roll_damage(struct ranctx *, int, int);

static int
mob_combat_max_roll(int stat, int bonus)
{
	/* only players have bonuses */
	int base_num = bonus != 0 ? 8 : 0;
	return (base_num + stat) * (64 + bonus);
}

static int
mob_combat_roll_damage(struct ranctx *ran, int stat, int bonus)
{
	int max = mob_combat_max_roll(stat, bonus);
	float f = ranval(ran) / (float)UINT32_MAX;
	float roll = max * f;
	return (int)((roll + 320.0f) / 640.0f);
}

int
mob_combat_roll(struct ranctx *ran, int att_stat, int att_bonus,
    int def_stat, int def_bonus, int str_stat, int str_bonus)
{
	int att_roll, def_roll;

	att_roll = ranval(ran) % mob_combat_max_roll(att_stat, att_bonus);
	def_roll = ranval(ran) % mob_combat_max_roll(def_stat, def_bonus);
	if (att_roll > def_roll) {
		return mob_combat_roll_damage(ran, str_stat, str_bonus);
	}
	return 0;
}

int
mob_wilderness_level(struct mob *mob)
{
	int depth = 2203 - (mob->y + PLANE_HEIGHT);
	if ((mob->x + PLANE_WIDTH) >= 2640) {
		depth = -50;
	}
	if (depth > 0) {
		return 1 + (depth / 6);
	}
	return depth;
}

void
mob_combat_reset(struct mob *mob)
{
	if (mob->dir == MOB_DIR_COMBAT_LEFT ||
	    mob->dir == MOB_DIR_COMBAT_RIGHT) {
		mob->dir = MOB_DIR_NORTH;
	}
	mob->in_combat = false;
	mob->combat_rounds = 0;
	mob->target_player = -1;
	mob->target_npc = -1;
}

bool
mob_within_range(struct mob *mob, int x, int y, int range)
{
	int x1 = x - range;
	int y1 = y - range;
	int x2 = x + range;
	int y2 = y + range;
	return mob->x > x1 && mob->y > y1 && mob->x < x2 && mob->y < y2;
}

size_t
get_nearby_players(struct mob *mob,
		   struct player **list, size_t max, int radius)
{
	int max_id = mob->server->max_player_id;
	size_t count = 0;

	for (int i = 0; i < max_id; ++i) {
		struct player *p2 = mob->server->players[i];
		if (p2 == NULL) {
			continue;
		}
		if (p2->name == -1) {
			continue;
		}
		if (count >= max) {
			break;
		}
		if (mob_within_range(mob, p2->mob.x, p2->mob.y, radius)) {
			list[count++] = p2;
		}
	}
	return count;
}

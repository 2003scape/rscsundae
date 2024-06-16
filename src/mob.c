#include <assert.h>
#include <math.h>
#include "entity.h"
#include "server.h"
#include "zone.h"

static int mob_combat_max_roll(int, int);
static int mob_combat_roll_damage(struct ranctx *, int, int);

static int
mob_combat_max_roll(int stat, int bonus)
{
	return stat * (64 + bonus);
}

static int
mob_combat_roll_damage(struct ranctx *ran, int stat, int bonus)
{
	int max = mob_combat_max_roll(stat, bonus);
	float f = ranval(ran) / (float)UINT32_MAX;
	float roll = max * f;
	return (int)((roll + 320.0f) / 640.0f);
}

void
mob_face(struct mob *mob, int x, int y)
{
	int dif_x = mob->x - x;
	int dif_y = mob->y - y;

	if (dif_x == 0) {
		if (dif_y > 0) {
			mob->dir = MOB_DIR_NORTH;
		} else if (dif_y < 0) {
			mob->dir = MOB_DIR_SOUTH;
		}
	} else if (dif_x < 0) {
		if (dif_y == 0) {
			mob->dir = MOB_DIR_WEST;
		} else if (dif_y < 0) {
			mob->dir = MOB_DIR_SOUTHWEST;
		} else if (dif_y > 0) {
			mob->dir = MOB_DIR_NORTHWEST;
		}
	} else if (dif_x > 0) {
		if (dif_y == 0) {
			mob->dir = MOB_DIR_EAST;
		} else if (dif_y < 0) {
			mob->dir = MOB_DIR_SOUTHEAST;
		} else if (dif_y > 0) {
			mob->dir = MOB_DIR_NORTHEAST;
		}
	}
}

uint32_t
mob_combat_xp(struct mob *mob)
{
	/* combat experience reward for killing a mob */
	int xp;
	int attack = 2 * mob->base_stats[SKILL_ATTACK];
	int defense = 2 * mob->base_stats[SKILL_DEFENSE];
	int strength = 2 * mob->base_stats[SKILL_STRENGTH];
	int hits = mob->base_stats[SKILL_HITS];

	xp = (uint32_t)((hits + attack + defense + strength) / 7.0) * 2 + 20;
	return (uint32_t)xp;
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
get_nearby_npcs(struct mob *mob,
		struct npc **list, size_t max, int radius)
{
	int max_id = mob->server->max_npc_id;
	size_t count = 0;

	for (int i = 0; i < max_id; ++i) {
		struct npc *npc = mob->server->npcs[i];
		if (npc == NULL) {
			continue;
		}
		if (count >= max) {
			break;
		}
		if (mob_within_range(mob, npc->mob.x, npc->mob.y, radius)) {
			list[count++] = npc;
		}
	}
	return count;
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

size_t
mob_get_nearby_locs(struct mob *mob,
		    struct loc *list, size_t max)
{
	size_t count = 0;
	struct zone *orig;
	struct zone *zone;

	orig = server_find_zone(mob->x, mob->y);
	if (orig != NULL) {
		for (int i = 0; i < orig->loc_count && count < max; ++i) {
			list[count++] = orig->locs[i];
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
			for (int i = 0; i < zone->loc_count &&
			    count < max; ++i) {
				list[count++] = zone->locs[i];
			}
		}
	}

	return count;
}

size_t
mob_get_nearby_bounds(struct mob *mob,
		      struct bound *list, size_t max)
{
	size_t count = 0;
	struct zone *orig;
	struct zone *zone;

	orig = server_find_zone(mob->x, mob->y);
	if (orig != NULL) {
		for (int i = 0; i < orig->bound_count && count < max; ++i) {
			list[count++] = orig->bounds[i];
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
			for (int i = 0; i < zone->bound_count &&
			    count < max; ++i) {
				list[count++] = zone->bounds[i];
			}
		}
	}

	return count;
}

void
mob_die(struct mob *mob)
{
	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		mob->cur_stats[i] = mob->base_stats[i];
	}

	mob->combat_timer = 0;
	mob->damage_timer = 0;

	mob_combat_reset(mob);
}

bool
mob_reached_item(struct mob *mob, struct ground_item *item)
{
	if (!item->on_surface) {
		return mob->x == item->x && mob->y == item->y;
	}
	return mob_within_range(mob, item->x, item->y, 2);
}

bool
mob_reached_bound(struct mob *mob, struct bound *bound)
{
	if (mob->x == bound->x && mob->y == bound->y) {
		return true;
	}
	switch (bound->dir) {
	case BOUND_DIR_HORIZ:
		if (mob->x == (bound->x - 1) && mob->y == bound->y) {
			return true;
		}
		return false;
	case BOUND_DIR_VERT:
		if (mob->x == bound->x && mob->y == (bound->y - 1)) {
			return true;
		}
		return false;
	case BOUND_DIR_DIAG_NW_SE:
		return mob_within_range(mob, bound->x, bound->y, 2);
	case BOUND_DIR_DIAG_NE_SW:
		return mob_within_range(mob, bound->x, bound->y, 2);
	}
	assert(0);
	return false;
}

bool
mob_reached_loc(struct mob *mob, struct loc *loc)
{
	struct loc_config *config;
	int max_x, max_y;

	config = server_loc_config_by_id(loc->id);
	assert(config != NULL);

	if (loc->dir == 0 || loc->dir == 4) {
		max_x = loc->x + config->width;
		max_y = loc->y + config->height;
	} else {
		max_x = loc->x + config->height;
		max_y = loc->y + config->width;
	}

	for (int x = loc->x; x < max_x; ++x) {
		for (int y = loc->y; y < max_y; ++y) {
			if (mob_within_range(mob, x, y, 2)) {
				return true;
			}
		}
	}

	return false;
}

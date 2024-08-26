#include <assert.h>
#include <strings.h>
#include <math.h>
#include "entity.h"
#include "server.h"
#include "zone.h"

static int mob_combat_max_roll(int, int);
static int mob_combat_roll_damage(int, int);
static bool mob_check_collision(struct mob *, int, int, int, int, int, bool);
static void mob_next_step(int, int, int, int, int, int *, int *, int *);

static int
mob_combat_max_roll(int stat, int bonus)
{
	return stat * (64 + bonus);
}

static int
mob_combat_roll_damage(int stat, int bonus)
{
	int max = mob_combat_max_roll(stat, bonus);
	double roll = max * server_random();
	return (int)((roll + 320.0) / 640.0);
}

void
mob_face(struct mob *mob, int x, int y)
{
	int unused_x;
	int unused_y;
	int dir;

	if (mob->moved) {
		/*
		 * can't move and change direction in same tick,
		 * or protocol breaks
		 */
		return;
	}

	mob_next_step(mob->x, mob->y, mob->dir,
	    x, y, &unused_x, &unused_y, &dir);

	mob->dir = dir;
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
mob_combat_roll(int att_stat, int att_bonus,
    int def_stat, int def_bonus, int str_stat, int str_bonus)
{
	int att_roll, def_roll;

	att_roll = mob_combat_max_roll(att_stat, att_bonus) * server_random();
	def_roll = mob_combat_max_roll(def_stat, def_bonus) * server_random();
	if (att_roll > def_roll) {
		return mob_combat_roll_damage(str_stat, str_bonus);
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
	mob->following_player = -1;
	mob->following_npc = -1;
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

int
mob_distance(struct mob *mob, int x, int y)
{
	return hypot(abs(x - (int)mob->x), abs(y - (int)mob->y));
}

size_t
mob_get_nearby_npcs(struct mob *mob,
		struct npc **list, size_t max, bool exclude_busy)
{
	struct npc *tmp[128];
	struct zone *orig, *zone;
	size_t count = 0;
	size_t tmp_count;

	orig = server_find_zone(mob->x, mob->y);
	if (orig == NULL) {
		return 0;
	}

	tmp_count = zone_find_npcs(orig, mob->server, tmp, 128, exclude_busy);
	for (size_t i = 0; i < tmp_count && count < max; ++i) {
		if (tmp[i]->respawn_time == 0) {
			list[count++] = tmp[i];
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
			if (count >= max) {
				break;
			}
			tmp_count = zone_find_npcs(zone,
			    mob->server, tmp, 128, exclude_busy);
			for (size_t i = 0; i < tmp_count && count < max; ++i) {
				/* keep within protocol limits */
				if (!mob_within_range(mob,
				    tmp[i]->mob.x, tmp[i]->mob.y, 16)) {
					continue;
				}
				if (tmp[i]->respawn_time == 0) {
					list[count++] = tmp[i];
				}
			}
		}
	}

	return count;
}

struct npc *
mob_find_nearby_npc(struct mob *mob, const char *name)
{
	struct npc *npcs[16];
	size_t n;

	n = mob_get_nearby_npcs(mob, npcs, 16, true);

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < npcs[i]->config->name_count; ++j) {
			if (strcasecmp(name, npcs[i]->config->names[j]) == 0) {
				return npcs[i];
			}
		}
	}
	return NULL;
}

size_t
mob_get_nearby_players(struct mob *mob, struct player **list, size_t max)
{
	struct player *tmp[128];
	struct zone *orig, *zone;
	size_t count = 0;
	size_t tmp_count = 0;

	orig = server_find_zone(mob->x, mob->y);
	if (orig == NULL) {
		return 0;
	}

	tmp_count = zone_find_players(orig, mob->server, tmp, 128);
	for (size_t i = 0; i < tmp_count && count < max; ++i) {
		list[count++] = tmp[i];
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
			if (count >= max) {
				break;
			}
			tmp_count = zone_find_players(zone,
			    mob->server, tmp, 128);
			for (size_t i = 0; i < tmp_count && count < max; ++i) {
				/* keep within protocol limits */
				if (mob_within_range(mob,
				    tmp[i]->mob.x, tmp[i]->mob.y, 16)) {
					list[count++] = tmp[i];
				}
			}
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
	if (orig == NULL) {
		return 0;
	}

	for (int i = 0; i < orig->loc_count && count < max; ++i) {
		list[count++] = orig->locs[i];
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
	if (orig == NULL) {
		return 0;
	}

	for (int i = 0; i < orig->bound_count && count < max; ++i) {
		list[count++] = orig->bounds[i];
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

	if (mob->target_player != -1) {
		struct player *target = mob->server->players[mob->target_player];
		if (target != NULL) {
			mob_combat_reset(&target->mob);
		}
	}

	if (mob->target_npc != -1) {
		struct npc *target = mob->server->npcs[mob->target_npc];
		if (target != NULL) {
			mob_combat_reset(&target->mob);
		}
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

void
mob_process_walk_queue(struct mob *mob)
{
	struct mob *following = NULL;
	int plane = 0;
	int dir;
	int x, y;

	if (mob->in_combat) {
		if (mob->dir == MOB_DIR_COMBAT_LEFT ||
		    mob->dir == MOB_DIR_COMBAT_RIGHT) {
			mob->walk_queue_len = 0;
			mob->walk_queue_pos = 0;
			return;
		}
	}

	if (mob->following_player != -1) {
		struct player *p;

		p = mob->server->players[mob->following_player];
		if (p != NULL) {
			following = &p->mob;
		} else {
			mob->following_player = -1;
		}
	} else if (mob->following_npc != -1) {
		struct npc *npc;

		npc = mob->server->npcs[mob->following_npc];
		if (npc != NULL) {
			following = &npc->mob;
		} else {
			mob->following_npc = -1;
		}
	}

	if (following != NULL) {
		if (mob_within_range(mob, following->x, following->y, 2)) {
			return;
		}
		if (!mob_within_range(mob, following->x, following->y, 3)) {
			mob->walk_queue_x[0] = following->x;
			mob->walk_queue_y[0] = following->y;
			mob->walk_queue_pos = 0;
			mob->walk_queue_len = 1;
		}
	}

	for (int tmp_y = mob->y; tmp_y >= PLANE_LEVEL_INC; plane++) {
		tmp_y -= PLANE_LEVEL_INC;
	}

	int pos = mob->walk_queue_pos;
	int remaining = mob->walk_queue_len - pos;
	if (remaining == 0) {
		mob->walk_queue_pos = 0;
		mob->walk_queue_len = 0;
		return;
	}

	mob_next_step(mob->x, mob->y, mob->dir,
	    mob->walk_queue_x[pos], mob->walk_queue_y[pos],
	    &x, &y, &dir);

	if (mob_check_collision(mob, mob->x, mob->y, x, y, dir, false)) {
		if (mob->in_combat) {
			/*
			 * if we're "in combat" while walking towards a mob,
			 * they're "caught".  do not enter combat and break
			 * things if we can't actually reach them.
			 */
			if (mob->target_player != -1) {
				struct player *p;

				p = mob->server->players[mob->target_player];
				if (p != NULL) {
					mob_combat_reset(&p->mob);
				}
			} else if (mob->target_npc != -1) {
				struct npc *npc;

				npc = mob->server->npcs[mob->target_npc];
				if (npc != NULL) {
					mob_combat_reset(&npc->mob);
				}
			}
			mob_combat_reset(mob);
		}
		mob->walk_queue_pos = 0;
		mob->walk_queue_len = 0;
		return;
	}

	if (x == mob->walk_queue_x[pos] &&
	    y == mob->walk_queue_y[pos]) {
		if (mob->walk_queue_pos + 1 == mob->walk_queue_len &&
		    !mob->is_npc && !mob->action_walk &&
		    (server_npc_on_tile(mob->server, x, y, false) ||
		    server_player_on_tile(mob->server, x, y))) {
			mob->walk_queue_pos = 0;
			mob->walk_queue_len = 0;
			mob_face(mob, x, y);
			return;
		}

		mob->walk_queue_pos++;
	}

	if (x != mob->x || y != mob->y) {
		mob->x = x;
		mob->y = y;
		mob->dir = dir;
		mob->moved = true;
		mob->movement_timer = 0;
	}
}

static bool
mob_check_collision(struct mob *mob,
    int cur_x, int cur_y, int x, int y, int dir,
    bool sight)
{
	int block, block_v, block_h;
	int plane = cur_y / PLANE_LEVEL_INC;

	int ty = y;
	int cy = cur_y;

	cy -= (plane * PLANE_LEVEL_INC);
	ty -= (plane * PLANE_LEVEL_INC);

	if (sight) {
		block = ADJ_BLOCK_SIGHT;
		block_v = ADJ_BLOCK_SIGHT_VERT;
		block_h = ADJ_BLOCK_SIGHT_HORIZ;
	} else {
		block = ADJ_BLOCK;
		block_v = ADJ_BLOCK_VERT;
		block_h = ADJ_BLOCK_HORIZ;
	}

	if (x >= ZONE_MAX_X || ty >= ZONE_MAX_Y || plane >= ZONE_MAX_PLANE) {
		return true;
	}

	struct server *s = mob->server;

	if ((s->adjacency[plane][x][ty] & block) != 0) {
		return true;
	}

	/*
	 * Players are prevented from walking through other players,
	 * UNLESS they have an action to perform on the tile. This can
	 * be seen in the documentation for ixBot (it used a strategy
	 * of keeping bones in inventory, to drop and pick up to regain
	 * access to occupied tiles).
	 *
	 * The same applies to aggressive NPCs.
	 */
	if (!sight && !mob->action_walk &&
	    (server_npc_on_tile(mob->server, x, y, true))) {
		return true;
	}

	if (mob->is_npc && !sight && !mob->action_walk &&
	    (server_npc_on_tile(mob->server, x, y, false) ||
	    server_player_on_tile(mob->server, x, y))) {
		return true;
	}

	switch (dir) {
	case MOB_DIR_NORTH:
		if ((s->adjacency[plane][x][ty] & block_v) != 0) {
			return true;
		}
		break;
	case MOB_DIR_SOUTH:
		if ((s->adjacency[plane][x][cy] & block_v) != 0) {
			return true;
		}
		break;
	case MOB_DIR_EAST:
		if ((s->adjacency[plane][x][ty] & block_h) != 0) {
			return true;
		}
		break;
	case MOB_DIR_WEST:
		if ((s->adjacency[plane][cur_x][ty] & block_h) != 0) {
			return true;
		}
		break;
	case MOB_DIR_NORTHWEST:
		/* check for cutting across two blocked tiles */
		if ((s->adjacency[plane][cur_x + 1][cy] & block) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy - 1] & block) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy] & block_h) != 0 &&
		    (s->adjacency[plane][cur_x][cy - 1] & block_v) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy] & block_h) != 0 &&
		    (s->adjacency[plane][cur_x][cy - 1] & block_h) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x + 1][cy - 1] & block_v) != 0 &&
		    (s->adjacency[plane][cur_x][cy - 1] & block_h) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x + 1][cy - 1] & block_v) != 0 &&
		    (s->adjacency[plane][cur_x][cy - 1] & block_v) != 0) {
			return true;
		}
		break;
	case MOB_DIR_NORTHEAST:
		/* check for cutting across two blocked tiles */
		if ((s->adjacency[plane][cur_x - 1][cy] & block) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy - 1] & block) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x - 1][cy - 1] & block_h) != 0 &&
		    (s->adjacency[plane][cur_x - 1][cy - 1] & block_v) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x - 1][cy] & block_h) != 0 &&
		    (s->adjacency[plane][cur_x - 1][cy - 1] & block_h) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x - 1][cy - 1] & block_v) != 0 &&
		    (s->adjacency[plane][cur_x][cy - 1] & block_v) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy - 1] & block_v) != 0 &&
		    (s->adjacency[plane][cur_x - 1][cy] & block_h) != 0) {
			return true;
		}
		break;
	case MOB_DIR_SOUTHWEST:
		/* check for cutting across two blocked tiles */
		if ((s->adjacency[plane][cur_x + 1][cy] & block) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy + 1] & block) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy] & block_h) != 0 &&
		    (s->adjacency[plane][cur_x][cy] & block_v) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy] & block_v) != 0 &&
		    (s->adjacency[plane][cur_x + 1][cy] & block_v) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy] & block_h) != 0 &&
		    (s->adjacency[plane][cur_x][cy + 1] & block_h) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x + 1][cy] & block_v) != 0 &&
		    (s->adjacency[plane][cur_x][cy + 1] & block_h) != 0) {
			return true;
		}
		break;
	case MOB_DIR_SOUTHEAST:
		/* check for cutting across two blocked tiles */
		if ((s->adjacency[plane][cur_x - 1][cy] & block) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy + 1] & block) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x - 1][cy + 1] & block_h) != 0 &&
		    (s->adjacency[plane][cur_x - 1][cy] & block_v) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy] & block_v) != 0 &&
		    (s->adjacency[plane][cur_x - 1][cy] & block_v) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x - 1][cy] & block_h) != 0 &&
		    (s->adjacency[plane][cur_x - 1][cy + 1] & block_h) != 0) {
			return true;
		}
		if ((s->adjacency[plane][cur_x][cy] & block_v) != 0 &&
		    (s->adjacency[plane][cur_x - 1][cy] & block_h) != 0) {
			return true;
		}
		break;
	}
	return false;
}

static void
mob_next_step(int cur_x, int cur_y, int cur_dir,
    int target_x, int target_y,
    int *x_out, int *y_out, int *dir_out)
{
	int dif_x = cur_x - target_x;
	int dif_y = cur_y - target_y;
	int x, y, dir;

	x = cur_x;
	y = cur_y;
	dir = cur_dir;

	if (dif_x == 0) {
		if (dif_y > 0) {
			dir = MOB_DIR_NORTH;
			y = cur_y - 1;
		} else if (dif_y < 0) {
			dir = MOB_DIR_SOUTH;
			y = cur_y + 1;
		}
	} else if (dif_x < 0) {
		if (dif_y == 0) {
			dir = MOB_DIR_WEST;
			x = cur_x + 1;
		} else if (dif_y < 0) {
			dir = MOB_DIR_SOUTHWEST;
			x = cur_x + 1;
			y = cur_y + 1;
		} else if (dif_y > 0) {
			dir = MOB_DIR_NORTHWEST;
			x = cur_x + 1;
			y = cur_y - 1;
		}
	} else if (dif_x > 0) {
		if (dif_y == 0) {
			dir = MOB_DIR_EAST;
			x = cur_x - 1;
		} else if (dif_y < 0) {
			dir = MOB_DIR_SOUTHEAST;
			x = cur_x - 1;
			y = cur_y + 1;
		} else if (dif_y > 0) {
			dir = MOB_DIR_NORTHEAST;
			x = cur_x - 1;
			y = cur_y - 1;
		}
	}

	*dir_out = dir;
	*x_out = x;
	*y_out = y;
}

bool
mob_check_reachable(struct mob *mob, int x, int y, bool sight)
{
	int cur_x = mob->x;
	int cur_y = mob->y;
	int cur_dir = mob->dir;

	while (cur_x != x || cur_y != y) {
		int prev_x = cur_x;
		int prev_y = cur_y;

		mob_next_step(cur_x, cur_y, cur_dir,
			x, y, &cur_x, &cur_y, &cur_dir);

		if (mob_check_collision(mob, prev_x, prev_y,
		    cur_x, cur_y, cur_dir, sight)) {
			return false;
		}
	}

	return true;
}

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "entity.h"
#include "server.h"
#include "utility.h"
#include "zone.h"

static struct zone *zones[ZONE_TOTAL_X][ZONE_TOTAL_Y][ZONE_MAX_PLANE] = {0};

static struct zone *zone_new(int, int, int);

static struct zone *
zone_new(int x, int y, int plane)
{
	struct zone *zone;

	zone = calloc(1, sizeof(struct zone));
	if (zone == NULL) {
		return NULL;
	}
	zone->x = x;
	zone->y = y;
	zone->plane = plane;
	zones[x][y][plane] = zone;
	return zone;
}

struct zone *
server_find_zone(int x, int y)
{
	struct zone *zone;
	int plane = 0;

	if (y > PLANE_LEVEL_INC) {
		plane = y / PLANE_LEVEL_INC;
		y -= (plane * PLANE_LEVEL_INC);
	}

	if (x < 0 || y < 0 || x >= ZONE_MAX_X || y >= ZONE_MAX_Y) {
		return NULL;
	}
	if (plane >= ZONE_MAX_PLANE) {
		return NULL;
	}

	x /= ZONE_WIDTH;
	y /= ZONE_WIDTH;

	zone = zones[x][y][plane];
	if (zone == NULL) {
		zone = zone_new(x, y, plane);
	}
	return zone;
}

struct zone *
server_get_zone(int x, int y, int plane)
{
	struct zone *zone;

	if (x < 1 || y < 1 || x >= ZONE_TOTAL_X || y >= ZONE_TOTAL_Y) {
		return NULL;
	}
	if (plane < 0 || plane >= ZONE_MAX_PLANE) {
		return NULL;
	}

	zone = zones[x][y][plane];
	if (zone == NULL) {
		zone = zone_new(x, y, plane);
	}
	return zone;
}

struct loc *
server_find_loc(int x, int y)
{
	struct zone *zone;

	zone = server_find_zone(x, y);
	if (zone == NULL) {
		return NULL;
	}
	/* TODO can optimize since one loc per tile */
	for (int i = 0; i < zone->loc_count; ++i) {
		if (zone->locs[i].x == x && zone->locs[i].y == y) {
			return &zone->locs[i];
		}
	}
	return NULL;
}

void
server_del_loc(int x, int y)
{
	struct zone *zone;

	zone = server_find_zone(x, y);
	if (zone == NULL) {
		return;
	}
	/* TODO can optimize since one loc per tile */
	for (int i = 0; i < zone->loc_count; ++i) {
		if (zone->locs[i].x != x || zone->locs[i].y != y) {
			continue;
		}
		zone->loc_count--;
		for (int j = i; j < zone->loc_count; ++j) {
			zone->locs[j] = zone->locs[j + 1];
		}
	}
}

void
server_add_loc(struct server *s, struct loc *loc)
{
	struct zone *zone;
	struct loc *old;
	struct loc_config *config;
	int x, y;
	int max_x, max_y;
	int y_dec;
	int plane;

	plane = 0;
	y = loc->y;
	while (y > PLANE_LEVEL_INC) {
		y -= PLANE_LEVEL_INC;
		plane++;
	}
	y_dec = PLANE_LEVEL_INC * plane;

	config = server_loc_config_by_id(loc->id);
	assert(config != NULL);

	if (loc->dir == MOB_DIR_NORTH ||
	    loc->dir == MOB_DIR_SOUTH) {
		max_x = loc->x + config->width;
		max_y = loc->y + config->height;
	} else {
		max_x = loc->x + config->height;
		max_y = loc->y + config->width;
	}

	old = server_find_loc(loc->x, loc->y);
	if (old != NULL) {
		for (x = loc->x; x < max_x; ++x) {
			for (y = loc->y; y < max_y; ++y) {
				s->adjacency[plane][x][y - y_dec] = 0;
			}
		}
	}

	for (x = loc->x; x < max_x; ++x) {
		for (y = loc->y; y < max_y; ++y) {
			if (config->type == LOC_TYPE_BLOCKING) {
				s->adjacency[plane][x][y - y_dec] |=
				    ADJ_BLOCK;
			} else if (config->type == LOC_TYPE_GATE_CLOSED) {
				switch (loc->dir) {
				case MOB_DIR_NORTH:
				case MOB_DIR_SOUTH:
					s->adjacency[plane][x][y - y_dec] |=
					    ADJ_BLOCK_HORIZ;
					break;
				case MOB_DIR_WEST:
				case MOB_DIR_EAST:
					s->adjacency[plane][x][y - y_dec] |=
					    ADJ_BLOCK_VERT;
					break;
				}
			}
			if (config->block_projectile) {
				s->adjacency[plane][x][y - y_dec] |=
				    ADJ_BLOCK_SIGHT;
			}
		}
	}

	if (old != NULL) {
		old->id = loc->id;
		return;
	}
	zone = server_find_zone(loc->x, loc->y);
	if (zone == NULL) {
		return;
	}
	if (zone->loc_count < ZONE_AREA) {
		memcpy(&zone->locs[zone->loc_count++],
		    loc, sizeof(struct loc));
	}
}

struct bound *
server_find_bound(int x, int y, int dir)
{
	struct zone *zone;

	zone = server_find_zone(x, y);
	if (zone == NULL) {
		return NULL;
	}
	for (int i = 0; i < zone->bound_count; ++i) {
		if (zone->bounds[i].x == x && zone->bounds[i].y == y &&
		    zone->bounds[i].dir == dir) {
			return &zone->bounds[i];
		}
	}
	return NULL;
}

void
server_add_bound(struct server *s, struct bound *bound)
{
	struct zone *zone;
	struct bound *old;
	struct bound_config *config;
	int plane = 0;
	int y;

	config = server_bound_config_by_id(bound->id);
	assert(config != NULL);

	y = bound->y;
	while (y > PLANE_LEVEL_INC) {
		y -= PLANE_LEVEL_INC;
		plane++;
	}

	old = server_find_bound(bound->x, bound->y, bound->dir);
	if (old != NULL) {
		switch (old->dir) {
		case BOUND_DIR_VERT:
			s->adjacency[plane][bound->x][y - 1] &=
			    ~ADJ_BLOCK_VERT;
			s->adjacency[plane][bound->x][y - 1] &=
			    ~ADJ_BLOCK_SIGHT_VERT;
			break;
		case BOUND_DIR_HORIZ:
			s->adjacency[plane][bound->x - 1][y] &=
			    ~ADJ_BLOCK_HORIZ;
			s->adjacency[plane][bound->x - 1][y] &=
			    ~ADJ_BLOCK_SIGHT_HORIZ;
			break;
		case BOUND_DIR_DIAG_NW_SE:
		case BOUND_DIR_DIAG_NE_SW:
			s->adjacency[plane][bound->x][y] = 0;
			break;
		}
	}

	switch (bound->dir) {
	case BOUND_DIR_VERT:
		if (config->block) {
			s->adjacency[plane][bound->x][y - 1] |=
			    ADJ_BLOCK_VERT;
		}
		if (config->block_projectile) {
			s->adjacency[plane][bound->x][y - 1] |=
			    ADJ_BLOCK_SIGHT_VERT;
		}
		break;
	case BOUND_DIR_HORIZ:
		if (config->block) {
			s->adjacency[plane][bound->x - 1][y] |=
			    ADJ_BLOCK_HORIZ;
		}
		if (config->block_projectile) {
			s->adjacency[plane][bound->x - 1][y] |=
			    ADJ_BLOCK_SIGHT_HORIZ;
		}
		break;
	case BOUND_DIR_DIAG_NW_SE:
	case BOUND_DIR_DIAG_NE_SW:
		if (config->block_projectile) {
			s->adjacency[plane][bound->x][y] |=
			    ADJ_BLOCK_SIGHT;
		}
		if (config->block) {
			s->adjacency[plane][bound->x][y] |= ADJ_BLOCK;
		}
		break;
	}

	if (old != NULL) {
		old->id = bound->id;
		return;
	}
	zone = server_find_zone(bound->x, bound->y);
	if (zone == NULL) {
		return;
	}
	if (zone->bound_count < ZONE_MAX_BOUNDS) {
		memcpy(&zone->bounds[zone->bound_count++],
		    bound, sizeof(struct bound));
	}
}

struct ground_item *
server_find_ground_item(struct player *p, int x, int y, int id)
{
	struct zone *zone;

	zone = server_find_zone(x, y);
	if (zone != NULL) {
		for (int i = 0; i < zone->item_count; ++i) {
			if (!player_can_see_item(p, &zone->items[i])) {
				continue;
			}
			if (zone->items[i].id == id &&
			    zone->items[i].x == x &&
			    zone->items[i].y == y) {
				return &zone->items[i];
			}
		}
	}

	for (size_t i = 0; i < p->mob.server->temp_item_count; ++i) {
		struct ground_item *item;

		item = &p->mob.server->temp_items[i];
		if (!player_can_see_item(p, item)) {
			continue;
		}
		if (item->id == id && item->x == x && item->y == y) {
			return &p->mob.server->temp_items[i];
		}
	}

	return NULL;
}

void
server_add_item_respawn(struct ground_item *item)
{
	struct zone *zone;

	assert(item != NULL);

	zone = server_find_zone(item->x, item->y);
	if (zone == NULL) {
		return;
	}
	if (zone->item_count >= zone->item_max) {
		uint16_t new_max = zone->item_max;

		if (new_max == 0) {
			new_max = 4;
		} else if (new_max == 4) {
			new_max = ZONE_AREA;
		} else {
			new_max *= 2;
		}
		if (reallocarr(&zone->items, new_max,
		    sizeof(struct ground_item)) == -1) {
			return;
		}
		zone->item_max = new_max;
	}
	memcpy(&zone->items[zone->item_count++],
	    item, sizeof(struct ground_item));
}

void
zone_add_player(struct zone *zone, uint16_t player_id)
{
	bool found_slot = false;

	for (size_t i = 0; i < zone->player_max; ++i) {
		if (zone->players[i] == UINT16_MAX) {
			/* unoccupied slot */
			zone->players[i] = player_id;
			return;
		}
	}

	uint16_t new_max = zone->player_max;

	if (new_max == 0) {
		new_max = 8;
	} else {
		new_max *= 2;
	}
	if (reallocarr(&zone->players, new_max, sizeof(uint16_t)) == -1) {
		return;
	}
	for (size_t i = zone->player_max; i < new_max; ++i) {
		if (found_slot) {
			zone->players[i] = UINT16_MAX;
		} else {
			zone->players[i] = player_id;
			found_slot = true;
		}
	}
	zone->player_max = new_max;
}

void
zone_add_npc(struct zone *zone, uint16_t npc_id)
{
	bool found_slot = false;

	for (size_t i = 0; i < zone->npc_max; ++i) {
		if (zone->npcs[i] == UINT16_MAX) {
			/* unoccupied slot */
			zone->npcs[i] = npc_id;
			return;
		}
	}

	uint16_t new_max = zone->npc_max;

	if (new_max == 0) {
		new_max = 2;
	} else {
		new_max *= 2;
	}
	if (reallocarr(&zone->npcs, new_max, sizeof(uint16_t)) == -1) {
		return;
	}
	for (size_t i = zone->npc_max; i < new_max; ++i) {
		if (found_slot) {
			zone->npcs[i] = UINT16_MAX;
		} else {
			zone->npcs[i] = npc_id;
			found_slot = true;
		}
	}
	zone->npc_max = new_max;
}

void
zone_remove_player(struct zone *zone, uint16_t player_id)
{
	for (size_t i = 0; i < zone->player_max; ++i) {
		if (zone->players[i] == player_id) {
			zone->players[i] = UINT16_MAX;
			return;
		}
	}
}

void
zone_remove_npc(struct zone *zone, uint16_t npc_id)
{
	for (size_t i = 0; i < zone->npc_max; ++i) {
		if (zone->npcs[i] == npc_id) {
			zone->npcs[i] = UINT16_MAX;
			return;
		}
	}
}

size_t
zone_find_npcs(struct zone *zone, struct server *s,
    struct npc **list, size_t max, bool exclude_busy)
{
	size_t count = 0;

	for (int i = 0; i < zone->npc_max; ++i) {
		if (zone->npcs[i] == UINT16_MAX) {
			continue;
		}
		struct npc *npc = s->npcs[zone->npcs[i]];
		if (npc == NULL) {
			zone->npcs[i] = UINT16_MAX;
			continue;
		}
		if (exclude_busy && (npc->busy || npc->mob.in_combat)) {
			continue;
		}
		if (count < max) {
			list[count++] = npc;
		} else {
			break;
		}
	}
	return count;
}

size_t
zone_find_players(struct zone *zone, struct server *s,
    struct player **list, size_t max)
{
	size_t count = 0;

	for (int i = 0; i < zone->player_max; ++i) {
		if (zone->players[i] == UINT16_MAX) {
			continue;
		}
		struct player *p = s->players[zone->players[i]];
		if (p == NULL) {
			zone->players[i] = UINT16_MAX;
		} else if (p->name != -1) {
			if (count < max) {
				list[count++] = p;
			} else {
				break;
			}
		}
	}
	return count;
}

bool
server_player_on_tile(struct server *s, int x, int y)
{
	struct zone *zone;
	struct player *p;

	zone = server_find_zone(x, y);
	if (zone == NULL) {
		return false;
	}

	for (size_t i = 0; i < zone->player_max; ++i) {
		if (zone->players[i] == UINT16_MAX) {
			continue;
		}

		p = s->players[zone->players[i]];
		if (p == NULL) {
			zone->players[i] = UINT16_MAX;
			continue;
		}
		if (p->mob.x == x && p->mob.y == y) {
			return true;
		}
	}
	return false;
}

bool
server_npc_on_tile(struct server *s, int x, int y, bool hard)
{
	struct npc *npc;
	struct zone *zone;

	zone = server_find_zone(x, y);
	if (zone == NULL) {
		return false;
	}

	for (size_t i = 0; i < zone->npc_max; ++i) {
		if (zone->npcs[i] == UINT16_MAX) {
			continue;
		}

		npc = s->npcs[zone->npcs[i]];
		if (npc == NULL) {
			zone->npcs[i] = UINT16_MAX;
			continue;
		}
		if (npc->respawn_time > 0) {
			continue;
		}
		if (hard && npc->config->aggression < 2) {
			continue;
		}
		if (npc->mob.x == x && npc->mob.y == y) {
			return true;
		}
	}
	return false;
}

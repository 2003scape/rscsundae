#include <stdlib.h>
#include <string.h>
#include "entity.h"
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
	int plane;

	plane = y / PLANE_LEVEL_INC;
	y -= (plane * PLANE_LEVEL_INC);

	if (x < 1 || y < 1 || x >= ZONE_MAX_X || y >= ZONE_MAX_Y) {
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
server_add_loc(struct loc *loc)
{
	struct zone *zone;
	struct loc *loc2;

	loc2 = server_find_loc(loc->x, loc->y);
	if (loc2 != NULL) {
		loc2->id = loc->id;
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
server_add_bound(struct bound *bound)
{
	struct zone *zone;
	struct bound *bound2;

	bound2 = server_find_bound(bound->x, bound->y, bound->dir);
	if (bound2 != NULL) {
		bound2->id = bound->id;
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

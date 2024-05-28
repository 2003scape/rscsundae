#include <math.h>
#include "entity.h"
#include "server.h"

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

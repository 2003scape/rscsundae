#ifndef ZONE_H
#define ZONE_H
#include <stdint.h>
#include <stddef.h>

#ifndef ZONE_WIDTH
#define ZONE_WIDTH	(8)
#endif

#ifndef ZONE_AREA
#define ZONE_AREA	(16)
#endif

#ifndef ZONE_MAX_X
#define ZONE_MAX_X	(450)
#endif

#ifndef ZONE_TOTAL_X
#define ZONE_TOTAL_X	(1 + (ZONE_MAX_X / ZONE_WIDTH))
#endif

#ifndef ZONE_MAX_Y
#define ZONE_MAX_Y	(750)
#endif

#ifndef ZONE_TOTAL_Y
#define ZONE_TOTAL_Y	(1 + (ZONE_MAX_Y / ZONE_WIDTH))
#endif

#ifndef ZONE_MAX_PLANE
#define ZONE_MAX_PLANE	(4)
#endif

/* 3d object in 2-dimensional space */
struct loc {
	uint16_t id;
	uint16_t x;
	uint16_t y;
	uint8_t dir;
};

/* small chunk of the game world */
struct zone {
	uint16_t x, y;
	uint8_t plane;
	struct loc locs[ZONE_AREA];
	size_t loc_count;
};

struct zone *server_find_zone(int, int);
struct zone *server_get_zone(int, int, int);
struct loc *server_find_loc(int, int);
void server_add_loc(struct loc *);

#endif

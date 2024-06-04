#ifndef JAG_MAP_H
#define JAG_MAP_H
#include <stdint.h>
#include <stddef.h>

#define JAG_MAP_CHUNK_SIZE	(48)
#define JAG_MAP_CHUNK_AREA	(48*48)

/* increment for inverse diagonal boundaries */
#define JAG_MAP_DIAG_INVERSE	(12000)

/* increment for NPCs boundaries */
#define JAG_MAP_DIAG_NPC	(24000)

/* increment for ground items */
#define JAG_MAP_DIAG_ITEM	(36000)

/* increment for locs stored inside bound_diag */
#define JAG_MAP_DIAG_LOC	(48000)

/* .jm files have a fixed size in bytes */
#define JAG_MAP_JM_FILE_LEN	(20736)

struct jag_tile {
	uint8_t height;
	uint8_t colour;
	uint8_t bound_vert;
	uint8_t bound_horiz;
	uint16_t bound_diag;
	uint8_t roof;
	uint8_t overlay;
	uint8_t loc_direction;
};

struct jag_map {
	struct jag_tile tiles[JAG_MAP_CHUNK_AREA];
};

int jag_map_read_jm(struct jag_map *, void *, size_t);
int jag_map_read_hei(struct jag_map *, void *, size_t);
int jag_map_read_loc(struct jag_map *, void *, size_t);
int jag_map_read_dat(struct jag_map *, void *, size_t, int);

int jag_map_write_jm(const struct jag_map *, void *, size_t);

#endif

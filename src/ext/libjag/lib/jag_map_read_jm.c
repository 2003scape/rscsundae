#include <map.h>
#include "buffer.h"

int
jag_map_read_jm(struct jag_map *out, void *b, size_t len) {
	size_t offset = 0;
	int prev = 0;
	uint8_t val = 0;
	uint16_t val_big = 0;
	unsigned i;

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		prev += val;
		out->tiles[i++].height = val & 0xff;
	}

	val = 0;

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		prev += val;
		out->tiles[i++].colour = val & 0xff;
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		out->tiles[i++].bound_horiz = val;
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		out->tiles[i++].bound_vert = val;
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu16(b, offset, len, &val_big) != 0) {
			return -1;
		}
		out->tiles[i++].bound_diag = val_big;
		offset += 2;
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		out->tiles[i++].roof = val;
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		out->tiles[i++].overlay = val;
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		out->tiles[i++].loc_direction = val;
	}

	return 0;
}

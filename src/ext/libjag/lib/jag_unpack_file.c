#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "jag.h"

#define READSZ	(4096)

/*
 * Read the header of a .jag archive and perform whole-archive
 * extraction if necessary.
 */
int
jag_unpack_file(FILE *f, struct jag_archive *out)
{
	uint8_t header[JAG_HEADER_SIZE];
	uint8_t *packed_data = NULL;
	uint8_t *unpacked_data = NULL;

	if (fread(header, 1, JAG_HEADER_SIZE, f) != JAG_HEADER_SIZE) {
		return -1;
	}

	if (jag_read_header(header, 0, JAG_HEADER_SIZE, out) == -1) {
		return -1;
	}

	if ((packed_data = malloc(out->packed_len)) == NULL) {
		return -1;
	}

	if (fread(packed_data, 1, out->packed_len, f) != out->packed_len) {
		free(packed_data);
		return -1;
	}

	/* not using archive-wide compression */
	if (out->unpacked_len == out->packed_len) {
		out->data = packed_data;
		out->must_free = 1;
		return 0;
	}

	unpacked_data = jag_bzip2_decompress(packed_data,
	    out->packed_len, out->unpacked_len);
	free(packed_data);
	if (unpacked_data != NULL) {
		out->data = unpacked_data;
		out->must_free = 1;
		return 0;
	}
	return -1;
}

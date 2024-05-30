#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <jag.h>

int
jag_unpack_entry(struct jag_entry *entry)
{
	uint8_t *unpacked_data = NULL;

	if (entry->packed_len == entry->unpacked_len) {
		/* already unpacked */
		return 0;
	}

	unpacked_data = jag_bzip2_decompress(entry->data,
	    entry->packed_len, entry->unpacked_len);
	if (unpacked_data != NULL) {
		if (entry->must_free) {
			free(entry->data);
		}
		entry->packed_len = entry->unpacked_len;
		entry->data = unpacked_data;
		entry->must_free = 1;
		return 0;
	}
	return -1;
}

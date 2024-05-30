#include "jag.h"
#include "buffer.h"

int
jag_read_entry(void *buf, size_t off, size_t len, struct jag_entry *out)
{
	if (jag_getu32(buf, off, len, &out->name_hash) != 0) {
		return -1;
	}
	off += 4;

	if (jag_getu24(buf, off, len, &out->unpacked_len) != 0) {
		return -1;
	}
	off += 3;

	if (jag_getu24(buf, off, len, &out->packed_len) != 0) {
		return -1;
	}
	off += 3;

	return JAG_ENTRY_HEADER_SIZE;
}

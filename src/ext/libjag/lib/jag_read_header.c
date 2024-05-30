#include "jag.h"
#include "buffer.h"

int
jag_read_header(void *buf, size_t off, size_t len, struct jag_archive *out)
{
	if (jag_getu24(buf, off, len, &out->unpacked_len) != 0) {
		return -1;
	}
	off += 3;

	if (jag_getu24(buf, off, len, &out->packed_len) != 0) {
		return -1;
	}
	off += 3;

	return JAG_HEADER_SIZE;
}

#include <bzlib.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "jag.h"

void *
jag_bzip2_decompress(void *buf, size_t packed_len, size_t unpacked_len)
{
	bz_stream bz = {0};
	uint8_t *packed_data = NULL;
	uint8_t *unpacked_data = NULL;
	int ret;

	if ((packed_data = malloc(packed_len +
	    sizeof(JAG_BZIP2_MAGIC) - 1)) == NULL) {
		goto fail;
	}

	/* fake a bzip2 header to satisfy the library */
	memcpy(packed_data, JAG_BZIP2_MAGIC, sizeof(JAG_BZIP2_MAGIC) - 1);

	memcpy(packed_data + sizeof(JAG_BZIP2_MAGIC) - 1, buf,
	    packed_len);

	unpacked_data = malloc(unpacked_len);
	if (unpacked_data == NULL) {
		goto fail;
	}

	bz.next_in = (char *)packed_data;
	bz.avail_in = packed_len + sizeof(JAG_BZIP2_MAGIC) - 1;
	bz.next_out = (char *)unpacked_data;
	bz.avail_out = unpacked_len;

	if (BZ2_bzDecompressInit(&bz, 0, 0) != BZ_OK) {
		goto fail;
	}

	while (bz.avail_out > 0) {
		ret = BZ2_bzDecompress(&bz);
		switch (ret) {
		case BZ_OK:
			continue;
		case BZ_STREAM_END:
			break;
		default:
			goto fail;
		}
	}

	BZ2_bzDecompressEnd(&bz);
	free(packed_data);
	return unpacked_data;
fail:
	BZ2_bzDecompressEnd(&bz);
	free(packed_data);
	free(unpacked_data);
	return NULL;
}

#include <sys/types.h>
#include <limits.h>
#include "buffer.h"

static const uint32_t bitmasks[] = {
	0, 0x1, 0x3, 0x7,
	0xf, 0x1f, 0x3f, 0x7f,
	0xff, 0x1ff, 0x3ff, 0x7ff,
	0xfff, 0x1fff, 0x3fff, 0x7fff,
	0xffff, 0x1ffff, 0x3ffff, 0x7ffff,
	0xfffff, 0x1fffff, 0x3fffff, 0x7fffff,
	0xffffff, 0x1ffffff, 0x3ffffff, 0x7ffffff,
	0xfffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
	0xffffffff
};

int
buf_getu8(void *b, size_t offset, size_t buflen, uint8_t *out)
{
	uint8_t *buffer = b;

	if (offset > (SIZE_MAX - 1) ||
	    ((ptrdiff_t)buflen - (ptrdiff_t)offset) < 1) {
		return -1;
	}
	*out = buffer[offset] & 0xff;
	return 0;
}

int
buf_getu16(void *b, size_t offset, size_t buflen, uint16_t *out)
{
	uint8_t *buffer = b;

	if (offset > (SIZE_MAX - 2) ||
	    ((ptrdiff_t)buflen - (ptrdiff_t)offset) < 2) {
		return -1;
	}
	*out = ((buffer[offset] & 0xff) << 8) |
		(buffer[offset + 1] & 0xff);
	return 0;
}

int
buf_getu32(void *b, size_t offset, size_t buflen, uint32_t *out)
{
	uint8_t *buffer = b;

	if (offset > (SIZE_MAX - 4) ||
	    ((ptrdiff_t)buflen - (ptrdiff_t)offset) < 4) {
		return -1;
	}
	*out = ((buffer[offset] & 0xff) << 24) |
	       ((buffer[offset + 1] & 0xff) << 16) |
	       ((buffer[offset + 2] & 0xff) << 8) |
	        (buffer[offset + 3] & 0xff);
	return 0;
}

int
buf_gets64(void *b, size_t offset, size_t buflen, int64_t *out)
{
	uint8_t *buffer = b;

	if (offset > (SIZE_MAX - 8) ||
	    ((ptrdiff_t)buflen - (ptrdiff_t)offset) < 8) {
		return -1;
	}
	*out = ((int64_t)(buffer[offset] & 0xff) << 56) |
		((int64_t)(buffer[offset + 1] & 0xff) << 48) |
		((int64_t)(buffer[offset + 2] & 0xff) << 40) |
		((int64_t)(buffer[offset + 3] & 0xff) << 32) |
		((int64_t)(buffer[offset + 4] & 0xff) << 24) |
		((int64_t)(buffer[offset + 5] & 0xff) << 16) |
		((int64_t)(buffer[offset + 6] & 0xff) << 8) |
	        (int64_t)(buffer[offset + 7] & 0xff);
	return 0;
}

int
buf_putu8(void *b, size_t offset, size_t buflen, uint8_t in)
{
	uint8_t *buffer = b;

	if (offset > (SSIZE_MAX - 1) ||
	    ((ssize_t)buflen - (ssize_t)offset) < 1) {
		return -1;
	}
	buffer[offset] = (uint8_t)(in);
	return 0;
}

int
buf_putu16(void *b, size_t offset, size_t buflen, uint16_t in)
{
	uint8_t *buffer = b;

	if (offset > (SSIZE_MAX - 2) ||
	    ((ssize_t)buflen - (ssize_t)offset) < 2) {
		return -1;
	}
	buffer[offset] = (uint8_t)(in >> 8);
	buffer[offset + 1] = (uint8_t)in;
	return 0;
}

int
buf_putu32(void *b, size_t offset, size_t buflen, uint32_t in)
{
	uint8_t *buffer;

	if (offset > (SSIZE_MAX - 4) ||
	    ((ssize_t)buflen - (ssize_t)offset) < 4) {
		return -1;
	}
	buffer = b + offset;
	*(buffer++) = (uint8_t)(in >> 24);
	*(buffer++) = (uint8_t)(in >> 16);
	*(buffer++) = (uint8_t)(in >> 8);
	*(buffer++) = (uint8_t)in;
	return 0;
}

int
buf_putu64(void *b, size_t offset, size_t buflen, uint64_t in)
{
	uint8_t *buffer;

	if (offset > (SSIZE_MAX - 8) ||
	    ((ssize_t)buflen - (ssize_t)offset) < 8) {
		return -1;
	}
	buffer = b + offset;
	*(buffer++) = (uint8_t)(in >> 56);
	*(buffer++) = (uint8_t)(in >> 48);
	*(buffer++) = (uint8_t)(in >> 40);
	*(buffer++) = (uint8_t)(in >> 32);
	*(buffer++) = (uint8_t)(in >> 24);
	*(buffer++) = (uint8_t)(in >> 16);
	*(buffer++) = (uint8_t)(in >> 8);
	*(buffer++) = (uint8_t)in;
	return 0;
}

int
buf_putbits(void *b, size_t bitpos, size_t buflen, int num, int value)
{
	uint8_t *buffer = b;
	size_t offset = bitpos >> 3;
	size_t i = 8 - (bitpos & 7);

	bitpos += num;

	for (; num > i; i = 8) {
		if (offset > (SSIZE_MAX - 2) ||
		    ((ssize_t)buflen - (ssize_t)offset) < 2) {
			return -1;
		}
		buffer[offset] &= ~bitmasks[i];
		buffer[offset++] |= (value >> (num - i)) & bitmasks[i];
		num -= i;
	}

	if (offset > (SSIZE_MAX - 1) ||
	    ((ssize_t)buflen - (ssize_t)offset) < 1) {
		return -1;
	}

	if (num == i) {
		buffer[offset] &= ~bitmasks[i];
		buffer[offset] |= value & bitmasks[i];
	} else {
		buffer[offset] &= ~(bitmasks[num] << (i - num));
		buffer[offset] |= (value & bitmasks[num]) << (i - num);
	}

	return 0;
}

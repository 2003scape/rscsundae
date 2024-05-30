#ifndef JAG_H
#define JAG_H
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

struct jag_archive {
	uint32_t unpacked_len;
	uint32_t packed_len;
	void *data;
	int must_free;
};

struct jag_entry {
	uint32_t name_hash;
	uint32_t unpacked_len;
	uint32_t packed_len;
	void *data;
	int must_free;
};

#define JAG_BZIP2_MAGIC			"BZh1"

/* 2x 3-byte sizes */
#define JAG_HEADER_SIZE			(6)

/* 2x 3-byte sizes, 1x 4-byte name hash */
#define JAG_ENTRY_HEADER_SIZE		(10)

#define JAG_PER_FILE_METADATA_START	(2)

/* high level functions, these return a jag_archive */
int jag_unpack_file(FILE *, struct jag_archive *);
int jag_unpack_stream(void *, size_t, size_t, struct jag_archive *);

/* high level functions */
int jag_find_entry(struct jag_archive *, const char *, struct jag_entry *);
int jag_unpack_entry(struct jag_entry *);

/* low level functions */
uint32_t jag_hash_entry_name(const char *);
int jag_read_header(void *, size_t, size_t, struct jag_archive *);
int jag_read_entry(void *, size_t, size_t, struct jag_entry *);
void *jag_bzip2_decompress(void *, size_t, size_t);

#endif

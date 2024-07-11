#ifndef UTILITY_H
#define UTILITY_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../config.h"

void username_sanitize(const char *, char *, size_t);
int64_t mod37_nameenc(const char *);
char *mod37_namedec(int64_t, char *);

void encode_chat_legacy(const char *, uint8_t *, size_t);
int chat_decompress(uint8_t *, size_t, size_t, char *);
int chat_compress(const char *, char *);

#ifndef HAVE_REALLOCARR
int reallocarr(void *, size_t, size_t);
#endif

#ifndef HAVE_ARC4RANDOM_BUF
void arc4random_buf(void *, size_t);
#endif

uint64_t get_time_ms(void);

char *buffer_file(const char *);

void gen_salt(unsigned char *, size_t);
#endif

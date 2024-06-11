#ifndef UTILITY_H
#define UTILITY_H
#include <stdint.h>
#include "../config.h"

char *mod37_namedec(int64_t, char *);

void encode_chat_legacy(const char *, uint8_t *, size_t);

#ifndef HAVE_REALLOCARR
int reallocarr(void *, size_t, size_t);
#endif

uint64_t get_time_ms(void);
#endif

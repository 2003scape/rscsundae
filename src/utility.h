#ifndef UTILITY_H
#define UTILITY_H
#include <stdint.h>
#include "../config.h"

char *mod37_namedec(int64_t, char *);

#ifndef HAVE_REALLOCARR
int reallocarr(void *, size_t, size_t);
#endif
#endif

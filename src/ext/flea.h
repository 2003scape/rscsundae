#ifndef FLEA_H
#define FLEA_H

#include <stdint.h>

struct ranctx {
	uint32_t a, b, c, d;
};

void raninit(struct ranctx *, uint32_t);

uint32_t ranval(struct ranctx *);

#endif

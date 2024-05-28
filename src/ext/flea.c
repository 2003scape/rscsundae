/*
 * small noncryptographic PRNG
 * public domain
 * https://burtleburtle.net/bob/rand/smallprng.html
 */

#include "flea.h"

#define rot(x,k) (((x)<<(k))|((x)>>(32-(k))))

uint32_t
ranval(struct ranctx *x)
{
	uint32_t e = x->a - rot(x->b, 27);
	x->a = x->b ^ rot(x->c, 17);
	x->b = x->c + x->d;
	x->c = x->d + e;
	x->d = e + x->a;
	return x->d;
}

void
raninit(struct ranctx *x, uint32_t seed)
{
	uint32_t i;
	x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
	for (i=0; i<20; ++i) {
		(void)ranval(x);
	}
}

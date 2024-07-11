#ifndef PWHASH_ARGON2_H
#define PWHASH_ARGON2_H
#include <stdint.h>

struct pwhash_params {
	uint32_t t_cost;
	uint32_t m_cost;
	uint32_t parallelism;
};

int pwhash_init(struct pwhash_params *);
int pwhash_hash(struct pwhash_params *,
    const char *, const char *, char *, size_t);
int pwhash_verify(struct pwhash_params *, const char *, const char *);

#endif

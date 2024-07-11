#include <argon2.h>
#include <stdlib.h>
#include <string.h>
#include "pwhash-argon2.h"
#include "../utility.h"

int
pwhash_init(struct pwhash_params *p)
{
	uint32_t time = 1;
	uint32_t mem = 4096;
	uint8_t salt[32];
	uint8_t password[32];
	uint8_t hash[256];
	uint64_t old_time;
	int res;

	/*
	 * fill with random data since we're just doing a speed
	 * calculation here
	 */
	arc4random_buf(salt, sizeof(salt));
	arc4random_buf(password, sizeof(password));

	for (;;) {
		old_time = get_time_ms();
		res = argon2d_hash_raw(time, mem, 1,
		    password, sizeof(password),
		    salt, sizeof(salt),
		    hash, sizeof(hash));
		mem *= 2;
		if (res != ARGON2_OK) {
			return -1;
		}
		if (mem > 32000 || (get_time_ms() - old_time) > 10) {
			break;
		}
	}

	for (int i = 0; i < 1000; ++i) {
		old_time = get_time_ms();
		res = argon2d_hash_raw(time, mem, 1,
		    password, sizeof(password),
		    salt, sizeof(salt),
		    hash, sizeof(hash));
		if (res != ARGON2_OK) {
			return -1;
		}
		if ((get_time_ms() - old_time) > 20) {
			break;
		}
		time++;
	}

	p->t_cost = time;
	p->m_cost = mem;
	p->parallelism = 1;
	return 0;
}

int
pwhash_hash(struct pwhash_params *p,
    const char *salt, const char *pwd, char *out, size_t outlen)
{
	int res;

	res = argon2id_hash_encoded(p->t_cost, p->m_cost, p->parallelism,
	    pwd, strlen(pwd), salt, strlen(salt), 64,
	    out, outlen);
	if (res != ARGON2_OK) {
		printf("%d\n", res);
		return -1;
	}
	return 0;
}

int
pwhash_verify(struct pwhash_params *p,
    const char *encoded, const char *pwd)
{
	int res;

	(void)p;

	res = argon2id_verify(encoded, pwd, strlen(pwd));
	if (res != ARGON2_OK) {
		return -1;
	}
	return 0;
}

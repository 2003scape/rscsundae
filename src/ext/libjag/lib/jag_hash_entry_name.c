#include <jag.h>
#include <ctype.h>
#include <string.h>

uint32_t
jag_hash_entry_name(const char *s)
{
	uint32_t hash = 0;
	size_t i = 0;
	size_t len;

	len = strlen(s);
	for (i = 0; i < len; ++i) {
		hash = hash * 61 + toupper((unsigned char)s[i]) - 32;
	}
	return hash;
}

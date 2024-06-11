#include "../config.h"
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "utility.h"

const char legacy_chartab[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', ' ', '!', '?', '.',
	',', ':', ';', '(', ')', '-', '&', '*', '\\', '\'',
	'\0'
};

static int encode_char_legacy(char);

static int
encode_char_legacy(char ch)
{
	if (isalpha(ch)) {
		ch = toupper(ch);
	}
	for (int i = 0; legacy_chartab[i] != '\0'; ++i) {
		if (legacy_chartab[i] == ch) {
			return i;
		}
	}
	return encode_char_legacy(' ');
}

void
encode_chat_legacy(const char *mes, uint8_t *out, size_t outlen)
{
	size_t inlen = strlen(mes);

	/*
	 * actual server for mudclient110 would also compress using
	 * goodwords.txt but we can probably afford to not do that
	 * in current year.
	 */
	for (size_t i = 0; i < inlen && i < outlen; ++i) {
		out[i] = (uint8_t)encode_char_legacy(mes[i]);
	}
}

char *
mod37_namedec(int64_t encoded, char *decoded)
{
	if (encoded < 0) {
		strcpy(decoded, "-invalid-");
		return decoded;
	}

	size_t length = 0;

	while (encoded != 0) {
		char c = (encoded % 37);
		encoded /= 37;

		if (c == 0) {
			decoded[length++] = ' ';
		} else if (c < 27) {
			if (encoded % 37 == 0) {
				decoded[length++] = (c + 65 - 1);
			} else {
				decoded[length++] = (c + 97 - 1);
			}
		} else {
			decoded[length++] = (c + 48 - 27);
		}
	}

	if (length == 0) {
		strcpy(decoded, "-invalid-");
		return decoded;
	}

	size_t end = length - 1;

	for (size_t i = 0; i < length / 2; i++) {
		char tmp = decoded[i];
		decoded[i] = decoded[end];
		decoded[end] = tmp;
		end--;
	}

	decoded[length++] = '\0';
	return decoded;
}

uint64_t
get_time_ms(void)
{
	uint64_t milliseconds = 0;
#ifdef HAVE_CLOCK_GETTIME
	struct timespec ts = {0};

	(void)clock_gettime(CLOCK_MONOTONIC, &ts);
	milliseconds = ts.tv_sec * 1000LL;
	milliseconds += ts.tv_nsec / 1000000LL;
#else
	struct timeval tv = {0};

	(void)gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec * 1000LL;
	milliseconds += tv.tv_usec / 1000LL;
#endif
	return milliseconds;
}

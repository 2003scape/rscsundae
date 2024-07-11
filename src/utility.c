#include "../config.h"
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifndef HAVE_ARC4RANDOM_BUF
#include <openssl/rand.h>
#endif
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "utility.h"

#ifndef MAX_CHAT_LEN
#define MAX_CHAT_LEN	(80)
#endif

static const char legacy_chartab[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', ' ', '!', '?', '.',
    ',', ':', ';', '(', ')', '-', '&', '*', '\\', '\'',
    '\0'
};


static const char compression_tab[] = {
    ' ', 'e', 't', 'a', 'o', 'i', 'h', 'n', 's', 'r',
    'd', 'l', 'u', 'm', 'w', 'c', 'y', 'f', 'g', 'p',
    'b', 'v', 'k', 'x', 'j', 'q', 'z', '0', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', ' ', '!', '?',
    '.', ',', ':', ';', '(', ')', '-', '&', '*', '\\',
    '\'', '@', '#', '+', '=', '\243', '$', '%', '"', '[',
    ']'
};

static int encode_char_legacy(char);

static int
encode_char_legacy(char ch)
{
	if (isalpha((unsigned char)ch)) {
		ch = toupper((unsigned char)ch);
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

void
username_sanitize(const char *name, char *out, size_t len)
{
	for (size_t i = 0; i < len; ++i) {
		if (isupper((unsigned char)name[i])) {
			out[i] = tolower((unsigned char)name[i]);
		} else if (isalnum((unsigned char)name[i])) {
			out[i] = name[i];
		} else {
			out[i] = ' ';
		}
	}
}

int64_t
mod37_nameenc(const char *name)
{
	int64_t encoded = 0;
	char sanitized[21];
	size_t len;

	len = strlen(name);
	if (len > 20) {
		return encoded;
	}

	username_sanitize(name, sanitized, len);
	sanitized[len + 1] = '\0';

	for (size_t i = 0; i < len; i++) {
		encoded *= 37;

		if (isalpha((unsigned char)sanitized[i])) {
		    encoded += 1 + sanitized[i] - 97;
		} else if (isdigit((unsigned char)sanitized[i])) {
		    encoded += 27 + sanitized[i] - 48;
		}
	}

	return encoded;
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

int
chat_decompress(uint8_t *buffer, size_t offset, size_t len, char *out)
{
	bool new_sentence = true;
	size_t new_length = 0;
	int left_shift = -1;

	for (size_t i = 0; i < len; i++) {
		int cur = buffer[offset++] & 0xff;
		int tab_index = cur >> 4 & 0xf;

		if (new_length >= MAX_CHAT_LEN) {
			break;
		}

		if (left_shift == -1) {
			if (tab_index < 13) {
				out[new_length++] = compression_tab[tab_index];
			} else {
				left_shift = tab_index;
			}
		} else {
			out[new_length++] =
			    compression_tab[((left_shift << 4) + tab_index) - 195];

			left_shift = -1;
		}

		tab_index = cur & 0xf;

		if (new_length >= MAX_CHAT_LEN) {
			break;
		}

		if (left_shift == -1) {
			if (tab_index < 13) {
				out[new_length++] = compression_tab[tab_index];
			} else {
				left_shift = tab_index;
			}
		} else {
			out[new_length++] =
			    compression_tab[((left_shift << 4) + tab_index) - 195];

			left_shift = -1;
		}
	}

	for (size_t i = 0; i < new_length; i++) {
		char ch = out[i];

		if (i > 4 && ch == '@') {
			out[i] = ' ';
		}

		if (ch == '%') {
			out[i] = ' ';
		}

		if (new_sentence && ch >= 'a' && ch <= 'z') {
			out[i] = toupper((unsigned char)out[i]);

			new_sentence = false;
		}

		if (ch == '.' || ch == '!') {
			new_sentence = true;
		}
	}

	out[new_length] = '\0';

	return new_length;
}

int
chat_compress(const char *input, char *output)
{
	size_t len = strlen(input);
	size_t offset = 0;
	int left_shift = -1;

	if (len > MAX_CHAT_LEN) {
		len = MAX_CHAT_LEN;
	}

	for (size_t i = 0; i < len; i++) {
		char ch = tolower((unsigned char)input[i]);
		size_t tab_index = 0;

		for (size_t j = 0; j < sizeof(compression_tab); j++) {
			if (ch == compression_tab[j]) {
				tab_index = j;
				break;
			}
		}

		if (tab_index > 12) {
			tab_index += 195;
		}

		if (left_shift == -1) {
			if (tab_index < 13) {
				left_shift = tab_index;
			} else {
				output[offset++] = tab_index & 0xff;
			}
		} else if (tab_index < 13) {
			output[offset++] = ((left_shift << 4) + tab_index) & 0xff;

			left_shift = -1;
		} else {
			output[offset++] =
				((left_shift << 4) + (tab_index >> 4)) & 0xff;

			left_shift = tab_index & 0xf;
		}
	}

	if (left_shift != -1) {
		output[offset++] = (left_shift << 4) & 0xff;
	}

	return offset;
}

char *
buffer_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "unable to open file %s\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(length + 1);
	if (buffer == NULL) {
		fprintf(stderr, "file buffer memory allocation failed\n");
		fclose(file);
		return NULL;
	}

	fread(buffer, 1, length, file);
	buffer[length] = '\0';
	fclose(file);

	return buffer;
}

#ifndef HAVE_ARC4RANDOM_BUF
void
arc4random_buf(void *buf, size_t len)
{
	if (RAND_bytes(buf, len) != 1) {
		fprintf(stderr, "fatal error with RAND_bytes\n");
		exit(EXIT_FAILURE);
	}
}
#endif

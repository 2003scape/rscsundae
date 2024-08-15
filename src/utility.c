#include "../config.h"
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifndef HAVE_ARC4RANDOM_BUF
#include <openssl/rand.h>
#endif
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "entity.h"
#include "server.h"
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
decode_chat_legacy(char **dict, size_t dict_len,
    uint8_t *mes, size_t inlen, char *out, size_t outlen)
{
	size_t off = 0;

	for (size_t i = 0; i < inlen; ++i) {
		uint8_t b = mes[i];
		if (b == 0xff) {
			/* initial header */
			if (i == (inlen - 1) || (outlen - off) < 6) {
				break;
			}
			uint8_t colour = mes[++i];
			switch (colour) {
			case 0:
				memcpy(out + off, "@red@", 5);
				off += 5;
				break;
			case 1:
				memcpy(out + off, "@gre@", 5);
				off += 5;
				break;
			case 2:
				memcpy(out + off, "@blu@", 5);
				off += 5;
				break;
			case 3:
				memcpy(out + off, "@cya@", 5);
				off += 5;
				break;
			case 4:
				memcpy(out + off, "@ran@", 5);
				off += 5;
				break;
			case 5:
				memcpy(out + off, "@whi@", 5);
				off += 5;
				break;
			case 6:
				memcpy(out + off, "@bla@", 5);
				off += 5;
				break;
			case 7:
				memcpy(out + off, "@ora@", 5);
				off += 5;
				break;
			case 8:
				memcpy(out + off, "@yel@", 5);
				off += 5;
				break;
			case 9:
				memcpy(out + off, "@mag@", 5);
				off += 5;
				break;
			}
		} else if (b < 50) {
			/* simple byte uncompressed */
			if (off == (outlen - 1)) {
				break;
			}
			out[off++] = tolower((unsigned char)legacy_chartab[b]);
		} else if (b < 70) {
			/* 2-byte pointer to word w/ final space */
			size_t len;
			uint16_t entry;

			if (off == (outlen - 1)) {
				break;
			}

			entry = ((b - 50) << 8) | mes[++i];
			if (entry >= dict_len) {
				continue;
			}
			len = strlen(dict[entry]);
			if ((outlen - off) <= len) {
				break;
			}
			memcpy(out + off, dict[entry], len);
			off += len;

			if (off == (outlen - 1)) {
				break;
			}
			out[off++] = ' ';
		} else if (b < 90) {
			/* 2-byte pointer to word w/o final space */
			size_t len;
			uint16_t entry;

			if (off == (outlen - 1)) {
				break;
			}

			entry = ((b - 70) << 8) | mes[++i];
			if (entry >= dict_len) {
				continue;
			}
			len = strlen(dict[entry]);
			if ((outlen - off) <= len) {
				break;
			}
			memcpy(out + off, dict[entry], len);
			off += len;
		} else {
			/* 1-byte pointer to word w/ final space */
			size_t len;

			if (b >= dict_len) {
				continue;
			}
			b -= 90;
			len = strlen(dict[b]);
			if ((outlen - off) <= len) {
				break;
			}
			memcpy(out + off, dict[b], len);
			off += len;

			if (off == (outlen - 1)) {
				break;
			}
			out[off++] = ' ';
		}
	}
	out[off++] = '\0';
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

void
gen_salt(unsigned char *salt, size_t len)
{
	arc4random_buf(salt, len);
	for (size_t i = 0; i < len; ++i) {
		salt[i] = '0' + (salt[i] % ('~' - '0'));
	}
	salt[len - 1] = '\0';
}

void
packet_log(struct player *p, const char *mes, ...)
{
	char path[FILENAME_MAX];
	char str[512];
	char name[32];
	struct tm tm = {0};
	time_t clock;
	va_list args;

	if (!p->mob.server->log_packets) {
		return;
	}

	clock = time(NULL);
	gmtime_r(&clock, &tm);

	if (p->packet_log == NULL) {
		(void)mkdir("./logs", S_IRWXU);
		mod37_namedec(p->name, name);
		(void)snprintf(path, FILENAME_MAX, "./logs/%s", name);
		(void)mkdir(path, S_IRWXU);
		strftime(str, sizeof(str), "%F-%H:%M", &tm);
		(void)snprintf(path, FILENAME_MAX, "./logs/%s/%s.txt", name, str);
		p->packet_log = fopen(path, "wa");
	}

	if (p->packet_log == NULL) {
		fprintf(stderr, "failed to open file %s: %s\n",
		    path, strerror(errno));
		return;
	}

	strftime(str, sizeof(str), "[%F %H:%M:%S] ", &tm);
	snprintf(str + strlen(str), sizeof(str), "%s", mes);

	va_start(args, mes);
	vfprintf(p->packet_log, str, args);
	va_end(args);
}

void
duration_to_str(time_t seconds, char *dest, size_t len)
{
	dest[0] = '\0';

	if (seconds >= 31536000) {
		time_t years = seconds / 31536000;
		seconds -= (years * 31536000);
		if (years > 1) {
			snprintf(dest, len, "%d years ", (int)years);
		} else {
			snprintf(dest, len, "1 year ");
		}
	}
	if (seconds >= 2592000) {
		time_t months = seconds / 2592000;
		seconds -= (months * 2592000);
		if (months > 1) {
			snprintf(dest + strlen(dest),
			    len, "%d months ", (int)months);
		} else {
			snprintf(dest + strlen(dest),
			    len, "1 month ");
		}
	}
	if (seconds >= 86400) {
		time_t days = seconds / 86400;
		seconds -= (days * 86400);
		if (days > 1) {
			snprintf(dest + strlen(dest), len,
			    "%d days ", (int)days);
		} else {
			snprintf(dest + strlen(dest), len, "1 day ");
		}
	}
	if (seconds >= 3600) {
		time_t hours = seconds / 3600;
		seconds -= (hours * 3600);
		if (hours > 1) {
			snprintf(dest + strlen(dest), len,
			    "%d hours ", (int)hours);
		} else {
			snprintf(dest + strlen(dest), len, "1 hour ");
		}
	}

	snprintf(dest + strlen(dest), len,
	    "%d minutes", (int)(seconds / 60));
}


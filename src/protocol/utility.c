#include <stddef.h>
#include <string.h>
#include "utility.h"

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

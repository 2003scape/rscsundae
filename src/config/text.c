#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "../buffer.h"
#include "config.h"

/* parser for the config46.jag txt format */

static ssize_t next_line(char *, size_t, size_t);
static ssize_t next_token(char *, size_t, size_t);
static ssize_t next_token_int(char *, size_t, size_t, long *);
static ssize_t next_token_hex(char *, size_t, size_t, unsigned long *);
static int find_entity(const char *, struct entity_config *, size_t);

static ssize_t
next_line(char *buffer, size_t offset, size_t len)
{
	char ch;
	uint8_t b;

	/* returns the start of the next line */

	while (buf_getu8((uint8_t *)buffer, offset++, len, &b) != -1) {
		ch = (char)b;

		if (offset == len) {
			return -1;
		}
		if (ch == '=') {
			return offset;
		}
	}
	return -1;
}

static ssize_t
next_token(char *buffer, size_t offset, size_t len)
{
	size_t nofs;
	char ch;
	uint8_t b;

	/* returns the start of the next token */

	while (buf_getu8((uint8_t *)buffer, offset++, len, &b) != -1) {
		ch = (char)b;

		if (offset > 1 && (ch == ',' || ch == ';')) {
			nofs = offset - 1;
			buffer[nofs] = '\0';
			do {
				nofs--;
			} while (nofs > 0 && buffer[nofs] != '\0' &&
			    buffer[nofs] != ',' && buffer[nofs] != ';' &&
			    buffer[nofs] != '=');
			return nofs + 1;
		}
	}
	return -1;
}

static ssize_t
next_token_int(char *buffer, size_t offset, size_t len, long *out)
{
	ssize_t n = next_token(buffer, offset, len);
	long result;

	/* returns the end of the int token and puts the result in out */

	if (n == -1) {
		return -1;
	}
	errno = 0;
	result = strtol(buffer + n, NULL, 10);
	if (errno != 0) {
		return -1;
	}
	*out = result;
	return n + strlen(buffer + n) + 1;
}

static ssize_t
next_token_hex(char *buffer, size_t offset, size_t len, unsigned long *out)
{
	ssize_t n = next_token(buffer, offset, len);
	unsigned long result;

	/* returns the end of the hex token and puts the reuslt in out */

	if (n == -1) {
		return -1;
	}
	errno = 0;
	result = strtoul(buffer + n, NULL, 16);
	if (errno != 0) {
		return -1;
	}
	*out = result;
	return n + strlen(buffer + n) + 1;
}

static int
find_entity(const char *name,
    struct entity_config *entities, size_t num_entities)
{
	if (strcasecmp(name, "na") == 0) {
		return -1;
	}
	for (size_t i = 0; i < num_entities; ++i) {
		if (strcasecmp(name, entities[i].name) == 0) {
			return (int)i;
		}
	}
	printf("warning: entity sprite not found: %s\n", name);
	return -1;
}

struct item_config *
config_parse_items(char *buffer, size_t len, size_t *num_items,
	struct entity_config *entities, size_t num_entities)
{
	struct item_config *items = NULL;
	size_t max_items;
	size_t offset;
	ssize_t tmp;
	long tmpl;
	unsigned long tmpul;

	max_items = 0;
	offset = 0;

	tmp = next_token_int(buffer, offset, len, &tmpl);
	if (tmp == -1) {
		return NULL;
	}
	offset = tmp;
	max_items = tmpl;

	items = calloc(max_items, sizeof(struct item_config));
	if (items == NULL) {
		return NULL;
	}

	for (size_t i = 0; i < max_items; ++i) {
		items[i].id = i;

		/* first line: names and description */

		tmp = next_line(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;

		tmpl = 0;
		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1 || tmpl > MAX_ITEM_NAMES) {
			goto err;
		}
		offset = tmp;
		items[i].name_count = tmpl;

		for (size_t j = 0; j < items[i].name_count; ++j) {
			tmp = next_token(buffer, offset, len);
			if (tmp == -1) {
				goto err;
			}
			items[i].names[j] = buffer + tmp;
			offset = tmp + strlen(buffer + tmp) + 1;
		}

		tmp = next_token(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		items[i].description = buffer + tmp;
		offset = tmp + strlen(buffer + tmp) + 1;

		/* second line: weight, value */

		tmp = next_line(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].sprite_id = tmpl;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].value = tmpl;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].weight = tmpl;

		tmp = next_token(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		items[i].option = buffer + tmp;
		offset = tmp + strlen((char *)buffer + tmp) + 1;

		/* third line: equipment bonuses and operation */

		tmp = next_line(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].bonus_aim = tmpl;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].bonus_armour = tmpl;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].bonus_magic = tmpl;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].bonus_power = tmpl;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].bonus_prayer = tmpl;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].bonus_hiding = tmpl;

		tmp = next_token(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		items[i].projectile = buffer + tmp;
		offset = tmp + strlen((char *)buffer + tmp) + 1;

		/* fourth line: animation, recolour, respawn time */

		tmp = next_line(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].equip_type = tmpl;

		tmp = next_token(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		items[i].entity_sprite = find_entity(buffer + tmp,
		    entities, num_entities);
		offset = tmp + strlen((char *)buffer + tmp) + 1;

		tmp = next_token_hex(buffer, offset, len, &tmpul);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].colour = tmpul;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].respawn_rate = tmpl;

		tmp = next_token_int(buffer, offset, len, &tmpl);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;
		items[i].quest_item = tmpl;
	}

	*num_items = max_items;
	return items;

err:
	assert(0);
	free(items);
	items = NULL;
	return NULL;
}

struct entity_config *
config_parse_entity(char *buffer, size_t len, size_t *num_entities)
{
	struct entity_config *entities = NULL;
	size_t max_entities;
	size_t offset;
	ssize_t tmp;
	long tmpl;

	offset = 0;
	tmp = next_token_int(buffer, offset, len, &tmpl);
	if (tmp == -1) {
		return NULL;
	}
	offset = tmp;
	max_entities = tmpl;

	entities = calloc(max_entities, sizeof(struct entity_config));
	if (entities == NULL) {
		return NULL;
	}

	for (size_t i = 0; i < max_entities; ++i) {
		entities[i].id = i;

		/* first line: name and filename */

		tmp = next_line(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;

		tmp = next_token(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		entities[i].name = buffer + tmp;
		offset = tmp + strlen((char *)buffer + tmp) + 1;

		tmp = next_token(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		entities[i].file_name = buffer + tmp;
		offset = tmp + strlen((char *)buffer + tmp) + 1;

		/* second line: colour mask and metadata */

		tmp = next_line(buffer, offset, len);
		if (tmp == -1) {
			goto err;
		}
		offset = tmp;

		/* ignored for now */
	}

	*num_entities = max_entities;
	return entities;
err:
	assert(0);
	free(entities);
	entities = NULL;
	return NULL;
}

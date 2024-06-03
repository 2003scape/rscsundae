#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_ITEM_NAMES	(4)
#define MAX_LOC_NAMES	(4)

/* entities defined in config.jag */

enum equip_type {
	EQUIP_TYPE_SHIELD	= 0x0008,
	EQUIP_TYPE_WEAPON	= 0x0010,
	EQUIP_TYPE_WEAPON_OFF	= 0x0018,
	EQUIP_TYPE_WEAPON_2H	= 0x2018,
	EQUIP_TYPE_HEAD		= 0x0020,
	EQUIP_TYPE_HEAD_FULL	= 0x0021,
	EQUIP_TYPE_TORSO	= 0x0040,
	EQUIP_TYPE_TORSO_FULL	= 0x0142,
	EQUIP_TYPE_HANDS	= 0x0100,
	EQUIP_TYPE_LEGS		= 0x0080,
	EQUIP_TYPE_LEGS_PASS	= 0x0280,
	EQUIP_TYPE_LEGS_FULL	= 0x0284,
	EQUIP_TYPE_SHOES	= 0x0200,
	EQUIP_TYPE_NECK		= 0x0400,
	EQUIP_TYPE_BACK		= 0x0800
};

/* "object.txt" - definitions for items */
struct item_config {
	uint16_t id;
	uint16_t name_count;
	char *names[MAX_ITEM_NAMES];
	char *description;
	uint16_t sprite_id;
	uint32_t value;
	uint8_t weight; /* > 0 == unstackable*/
	char *option;
	uint8_t bonus_aim;
	uint8_t bonus_armour;
	uint8_t bonus_magic;
	uint8_t bonus_power;
	uint8_t bonus_prayer;
	uint8_t bonus_hiding; /* not used */
	char *projectile;
	uint16_t equip_type;
	uint16_t entity_sprite;
	uint32_t colour;
	uint16_t respawn_rate;
	uint8_t quest_item; /* untradable */
};

/* "entity.txt" - definitions for sprites/animations for mobs */
struct entity_config {
	uint16_t id;
	char *name;
	char *file_name;
	uint32_t colour;
	uint8_t mask;
	uint8_t has_a;
	uint8_t has_f;
};

/* prayer.txt */
struct prayer_config {
	uint16_t id;
	char *name;
	uint8_t level;
	char *description;
	uint8_t drain;
};

/* "location.txt" - definitions for 3d objects */
struct loc_config {
	uint16_t id;
	uint16_t name_count;
	char *names[MAX_LOC_NAMES];
	char *description;
	char *model;
	uint8_t width;
	uint8_t height;
	uint8_t type;
	uint8_t unknown; /* XXX */
	char *option;
	char *option_alt;
	uint16_t surface_height;
};

struct item_config *config_parse_items(char *, size_t, size_t *,
    struct entity_config *, size_t);
struct entity_config *config_parse_entity(char *, size_t, size_t *);
struct prayer_config *config_parse_prayers(char *, size_t, size_t *);
struct loc_config *config_parse_locs(char *, size_t, size_t *);
bool item_equip_clear(struct item_config *, int);

#endif

#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <stddef.h>

#define MAX_ITEM_NAMES	(4)

/* entities defined in config.jag */

enum equip_type {
	EQUIP_TYPE_SHIELD	= 0x0008,
	EQUIP_TYPE_WEAPON	= 0x0010,
	EQUIP_TYPE_WEAPON_2H	= 0x2018,
	EQUIP_TYPE_HEAD		= 0x0020,
	EQUIP_TYPE_HEAD_FULL	= 0x0021,
	EQUIP_TYPE_TORSO	= 0x0040,
	EQUIP_TYPE_TORSO_FULL	= 0x0142,
	EQUIP_TYPE_GLOVES	= 0x0100,
	EQUIP_TYPE_LEG		= 0x0080,
	EQUIP_TYPE_LEG_FULL	= 0x0280,
	EQUIP_TYPE_LEG_FULL2	= 0x0284,
	EQUIP_TYPE_SHOES	= 0x0200,
	EQUIP_TYPE_NECK		= 0x0400,
	EQUIP_TYPE_BACK		= 0x0800
};

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
	char *animation;
	uint32_t colour;
	uint16_t respawn_rate;
	uint8_t quest_item; /* untradable */
};

struct item_config *
config_parse_items(char *, size_t, size_t *);

#endif

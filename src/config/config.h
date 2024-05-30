#ifndef CONFIG_H
#define CONFIG_H

#define MAX_ITEM_NAMES	(4)

/* entities defined in config.jag */

struct item_config {
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
	uint32_t equip_mask;
	char *animation;
	uint32_t colour;
	uint16_t respawn_rate;
	uint8_t quest_item; /* untradable */
};

#endif

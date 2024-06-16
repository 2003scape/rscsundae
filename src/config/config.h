#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_ITEM_NAMES	(4)
#define MAX_LOC_NAMES	(4)
#define MAX_BOUND_NAMES	(4)
#define MAX_NPC_NAMES	(4)

#define MAX_SPELL_REAGENTS	(4)

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

/* "boundary.txt" - definitions for 2d wall objects */
struct bound_config {
	uint16_t id;
	uint16_t name_count;
	char *names[MAX_BOUND_NAMES];
	char *description;
	uint16_t height;
	int32_t fill_front;
	int32_t fill_back;
	uint8_t block;
	uint8_t block_projectile;
	uint8_t interactive;
	char *option;
	char *option_alt;
};

enum projectile_type {
	PROJECTILE_TYPE_MAGIC	= 0,
	PROJECTILE_TYPE_RANGED	= 1,
};

/* "projectile.txt" */
struct projectile_config {
	uint16_t id;
	char *name;
	uint8_t sprite;
	uint8_t range;
	uint8_t aim;
	uint8_t power;
	char *item;
	uint8_t type;
};

enum move_restrict {
	MOVE_RESTRICT_NONE = 0,
	MOVE_RESTRICT_OUTDOORS = 1,
	MOVE_RESTRICT_INDOORS = 2,
};

struct npc_drop_config {
	uint16_t id;
	uint16_t amount;
};

/* npc.txt */
struct npc_config {
	uint16_t id;
	uint16_t name_count;
	char *names[MAX_NPC_NAMES];
	char *description;
	uint8_t attack;
	uint8_t defense;
	uint8_t strength;
	uint8_t hits;
	uint8_t aggression;
	uint8_t bravery;	/* hits at which to retreat */
	uint8_t regeneration;
	uint8_t perception;	/* unused */
	uint8_t wander_range;
	uint8_t move_restrict;
	uint16_t respawn;
	uint8_t unused1;
	uint8_t unused2;
	uint8_t hunt_range;
	uint8_t unused3;
	int16_t sprites[12];
	uint32_t colour_hair;
	uint32_t colour_top;
	uint32_t colour_bottom;
	uint32_t colour_skin;
	uint16_t width;
	uint16_t height;
	uint8_t walk_speed;	/* sprite */
	uint8_t combat_speed;
	uint8_t combat_width;
	uint16_t drop_count;
	struct npc_drop_config *drops;
};

enum spell_type {
	SPELL_CAST_ON_SELF	= 0,
	SPELL_CAST_ON_PLAYER	= 1,
	SPELL_CAST_ON_MOB	= 2,
	SPELL_CAST_ON_ITEM	= 3,
	SPELL_CAST_ON_BOUND	= 4,
	SPELL_CAST_ON_LOC	= 5,
	SPELL_CAST_ON_TILE	= 6,
};

struct spell_reagent {
	uint16_t item_id;
	uint8_t amount;
};

struct spell_config {
	uint16_t id;
	char *name;
	uint8_t level;
	char *description;
	uint8_t type;
	uint8_t reagent_count;
	struct spell_reagent reagents[MAX_SPELL_REAGENTS];
};

struct shop_item {
	uint16_t id;
	uint16_t quantity;
	uint16_t cur_quantity;
	uint16_t restock;
	uint16_t restock_timer;
	uint16_t removal_timer;
};

#define MAX_SHOP_ITEMS	(40)

struct shop_config {
	uint16_t id;
	char *name;
	uint8_t sell_modifier;
	uint8_t buy_modifier;
	uint8_t delta;
	uint16_t pawn_limit;
	uint8_t item_count;
	struct shop_item items[MAX_SHOP_ITEMS];
	uint8_t changed;
};

int config_find_entity(const char *, struct entity_config *, size_t);
int config_find_item(const char *, struct item_config *, size_t);
struct item_config *config_parse_items(char *, size_t, size_t *,
    struct entity_config *, size_t);
struct entity_config *config_parse_entity(char *, size_t, size_t *);
struct prayer_config *config_parse_prayers(char *, size_t, size_t *);
struct loc_config *config_parse_locs(char *, size_t, size_t *);
struct bound_config *config_parse_bounds(char *, size_t, size_t *);
struct projectile_config *config_parse_projectiles(char *, size_t, size_t *);
struct npc_config *config_parse_npcs(char *, size_t, size_t *,
    struct entity_config *, size_t, struct item_config *, size_t);
struct spell_config *config_parse_spells(char *, size_t, size_t *,
    struct item_config *, size_t);
struct shop_config *config_parse_shops(char *, size_t, size_t *,
    struct item_config *, size_t);
bool item_equip_clear(struct item_config *, int);

#endif

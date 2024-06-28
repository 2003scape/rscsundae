#ifndef SERVER_H
#define SERVER_H
#include <lua.h>
#include "config/config.h"
#include "protocol/rsa.h"
#include "ext/flea.h"
#include "entity.h"
#include "zone.h"

/*
 * Real server varied a lot but tended to aim towards 600 - many
 * replays show 600ms between script actions (e.g. mes).
 *
 * Older operating systems often cannot keep an exact time due to
 * waking up timers at regular intervals.  Sundae attempts to do
 * its best to compensate for this.  Also, virtual machines often
 * have unstable system clocks.
 */
#define TICK_RATE	(600)

#define MAXPLAYERS	(1000)
#define MAXNPCS		(1000)

#define ADJ_BLOCK_NORTH		(1 << 0)
#define ADJ_BLOCK_SOUTH		(1 << 1)
#define ADJ_BLOCK_EAST		(1 << 2)
#define ADJ_BLOCK_WEST		(1 << 3)
#define ADJ_BLOCK_SIGHT_NORTH	(1 << 4)
#define ADJ_BLOCK_SIGHT_SOUTH	(1 << 5)
#define ADJ_BLOCK_SIGHT_EAST	(1 << 6)
#define ADJ_BLOCK_SIGHT_WEST	(1 << 7)

struct server {
	void *loop_ctx;
	char *name;
	char *bind_addr;
	int port;
	uint16_t player_count;
	uint16_t max_player_id;
	uint16_t max_npc_id;
	uint64_t tick_counter;
	uint64_t next_prayer_drain;
	uint64_t next_restore;
	uint64_t next_rapid_restore;
	uint64_t last_tick;
	uint8_t adjacency[ZONE_MAX_PLANE][ZONE_MAX_X][ZONE_MAX_Y];
	uint8_t roofs[ZONE_MAX_X][ZONE_MAX_Y];
	struct player *players[MAXPLAYERS];
	struct npc *npcs[MAXNPCS];
	struct ground_item *temp_items;
	size_t temp_item_count;
	size_t temp_item_max;
	uint64_t ground_item_counter;
	struct ranctx ran;
	struct item_config *item_config;
	size_t item_config_count;
	struct entity_config *entity_config;
	size_t entity_config_count;
	struct prayer_config *prayer_config;
	size_t prayer_config_count;
	struct loc_config *loc_config;
	size_t loc_config_count;
	struct bound_config *bound_config;
	size_t bound_config_count;
	struct projectile_config *projectile_config;
	size_t projectile_config_count;
	struct npc_config *npc_config;
	size_t npc_config_count;
	struct spell_config *spell_config;
	size_t spell_config_count;
	struct shop_config *shop_config;
	size_t shop_config_count;
	struct floor_config *floor_config;
	size_t floor_config_count;
	uint16_t start_tile_x;
	uint16_t start_tile_y;
	/* emulate bug in original server where checks were clientside */
	bool cast_without_runes;
	/* original server had no spell timer until runebot was developed */
	bool spell_timer;
	/* https://classic.runescape.wiki/w/Update:Updates_galore! */
	bool restrict_npc_aggression;
	int xp_multiplier;
	lua_State *lua;
	struct rsa rsa;
	char *rsa_exponent;
	char *rsa_modulus;
};

void server_tick(void);
struct player *server_find_player_name37(int64_t);
bool server_has_player(int64_t);
void server_register_login(int64_t);
void server_register_logout(int64_t);
void server_register_hide_status(struct player *);
void server_register_unhide_status(struct player *);
void server_send_pm(struct player *, int64_t, uint8_t *, size_t);
struct floor_config *server_floor_config_by_id(int);
struct item_config *server_item_config_by_id(int);
struct item_config *server_find_item_config(const char *);
struct prayer_config *server_prayer_config_by_id(int);
struct loc_config *server_loc_config_by_id(int);
struct loc_config *server_find_loc_config(const char *);
struct bound_config *server_bound_config_by_id(int);
struct bound_config *server_find_bound_config(const char *);
struct npc_config *server_npc_config_by_id(int);
struct npc_config *server_find_npc_config(const char *);
struct spell_config *server_spell_config_by_id(int);
struct projectile_config *server_find_projectile(const char *);
struct shop_config *server_find_shop(const char *);
int server_add_npc(int, int, int);
int server_add_temp_item(struct player *, int, int, int, uint32_t);
void server_remove_temp_item(uint64_t);
int server_parse_settings(void *, const char *, const char *, const char *);
double server_random(void);

#endif

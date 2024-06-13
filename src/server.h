#ifndef SERVER_H
#define SERVER_H
#include <lua.h>
#include "config/config.h"
#include "ext/flea.h"
#include "entity.h"

#define MAXPLAYERS	(1000)
#define MAXNPCS		(1000)

struct server {
	void *loop_ctx;
	uint16_t player_count;
	uint16_t max_player_id;
	uint16_t max_npc_id;
	uint64_t tick_counter;
	uint64_t next_prayer_drain;
	uint64_t next_restore;
	uint64_t next_rapid_restore;
	uint64_t last_tick;
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
	uint16_t start_tile_x;
	uint16_t start_tile_y;
	int xp_multiplier;
	lua_State *lua;
};

void server_tick(void);
struct player *server_find_player_name37(int64_t);
bool server_has_player(int64_t);
void server_register_login(int64_t);
void server_register_logout(int64_t);
void server_register_hide_status(struct player *);
void server_register_unhide_status(struct player *);
void server_send_pm(struct player *, int64_t, uint8_t *, size_t);
struct item_config *server_item_config_by_id(int);
struct item_config *server_find_item_config(const char *);
struct prayer_config *server_prayer_config_by_id(int);
struct loc_config *server_loc_config_by_id(int);
struct bound_config *server_bound_config_by_id(int);
struct npc_config *server_npc_config_by_id(int);
struct spell_config *server_spell_config_by_id(int);
struct projectile_config *server_find_projectile(const char *);
int server_add_npc(int, int, int);
int server_add_temp_item(struct player *, int, int, int, uint32_t);
void server_remove_temp_item(uint64_t);

#endif

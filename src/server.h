#ifndef SERVER_H
#define SERVER_H
#include "config/config.h"
#include "ext/flea.h"
#include "entity.h"

#define MAXPLAYERS	(2000)

struct server {
	void *loop_ctx;
	uint16_t player_count;
	uint16_t max_player_id;
	uint64_t tick_counter;
	uint64_t next_prayer_drain;
	uint64_t next_restore;
	uint64_t next_rapid_restore;
	struct player *players[MAXPLAYERS];
	struct ranctx ran;
	struct item_config *item_config;
	size_t item_config_count;
	struct entity_config *entity_config;
	size_t entity_config_count;
	struct prayer_config *prayer_config;
	size_t prayer_config_count;
	int xp_multiplier;
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
struct prayer_config *server_prayer_config_by_id(int);

#endif

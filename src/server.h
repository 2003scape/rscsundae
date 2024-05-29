#ifndef SERVER_H
#define SERVER_H
#include "ext/flea.h"
#include "entity.h"

#define MAXPLAYERS	(2000)

struct server {
	void *loop_ctx;
	uint16_t player_count;
	uint16_t max_player_id;
	uint64_t tick_counter;
	struct player *players[MAXPLAYERS];
	struct ranctx ran;
};

void server_tick(void);
struct player *server_find_player_name37(int64_t);
void server_register_login(int64_t);
void server_register_logout(int64_t);
void server_register_hide_status(struct player *);
void server_register_unhide_status(struct player *);
void server_send_pm(struct player *, int64_t, uint8_t *, size_t);

#endif

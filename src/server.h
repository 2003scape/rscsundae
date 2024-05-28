#ifndef SERVER_H
#define SERVER_H
#include "ext/flea.h"
#include "entity.h"

#define MAXPLAYERS	(2000)

struct server {
	void *loop_ctx;
	uint16_t player_count;
	uint16_t max_player_id;
	struct player *players[MAXPLAYERS];
	struct ranctx ran;
};

void server_tick(void);

#endif

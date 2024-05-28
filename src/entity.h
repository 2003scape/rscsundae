#ifndef ENTITY_H
#define ENTITY_H
#include <stdint.h>

#define PLAYER_BUFSIZE		(5000)

#define PLANE_LEVEL_INC		(944)

struct mob {
	uint16_t x, y;
	uint16_t id;
	uint8_t dir;
	uint8_t prev_dir;
	uint8_t moved;
};

struct player {
	struct mob mob;
	int sock;
	uint8_t tmpbuf[PLAYER_BUFSIZE]; /* used for packet construction */
	uint8_t inbuf[PLAYER_BUFSIZE];
	uint16_t inbuf_len;
	uint16_t inbuf_processed;
	uint8_t outbuf[PLAYER_BUFSIZE];
	uint16_t outbuf_len;
	uint16_t outbuf_written;
	int64_t session_id;
	int64_t name;
};

struct bound {
	uint16_t id;
	uint16_t x, y;
	uint8_t dir;
};

struct loc {
	uint16_t id;
	uint16_t x, y;
	uint8_t dir;
};

struct server;

/* player.c */
struct player *player_accept(struct server *, int);

/* incoming.c */
int player_parse_incoming(struct player *);

/* outgoing.c */
int player_send_plane_init(struct player *);
int player_send_movement(struct player *);

#endif

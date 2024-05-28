#ifndef ENTITY_H
#define ENTITY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PLAYER_BUFSIZE		(5000)

#define PLANE_LEVEL_INC		(944)

#define MAX_KNOWN_PLAYERS	(255)
#define MAX_PUBLIC_CHAT_LEN	(80)
#define WALK_QUEUE_LEN		(16)

struct server;

enum mobdir {
	MOB_DIR_NORTH		= 0,
	MOB_DIR_NORTHWEST	= 1,
	MOB_DIR_WEST		= 2,
	MOB_DIR_SOUTHWEST	= 3,
	MOB_DIR_SOUTH		= 4,
	MOB_DIR_SOUTHEAST	= 5,
	MOB_DIR_EAST		= 6,
	MOB_DIR_NORTHEAST	= 7
};

struct mob {
	struct server *server;
	uint16_t x, y;
	uint16_t id;
	uint8_t dir;
	uint8_t prev_dir;
	uint8_t moved;
};

enum animslot {
	ANIM_SLOT_HEAD		= 0,
	ANIM_SLOT_BODY		= 1,
	ANIM_SLOT_LEGS		= 2,
	ANIM_SLOT_OFFHAND	= 3,
	ANIM_SLOT_HAND		= 4,
	ANIM_SLOT_HELMET	= 5,
	ANIM_SLOT_SHIRT		= 6,
	ANIM_SLOT_TROUSERS	= 7,
	ANIM_SLOT_NECK		= 8,
	ANIM_SLOT_SHOES		= 9,
	ANIM_SLOT_GLOVES	= 10,
	ANIM_SLOT_CAPE		= 11,
};

struct player {
	struct mob mob;
	int sock;
	struct player *known_players[MAX_KNOWN_PLAYERS];
	uint8_t known_players_seen[MAX_KNOWN_PLAYERS];
	uint16_t known_player_count;
	uint8_t tmpbuf[PLAYER_BUFSIZE]; /* used for packet construction */
	uint8_t inbuf[PLAYER_BUFSIZE];
	uint16_t inbuf_len;
	uint16_t inbuf_processed;
	uint8_t outbuf[PLAYER_BUFSIZE];
	uint16_t outbuf_len;
	uint16_t outbuf_written;
	int64_t session_id;
	int64_t name;
	uint8_t appearance_changed;
	uint8_t plane_changed;
	uint8_t moved;
	uint8_t sprites[12];
	uint8_t hair_colour;
	uint8_t top_colour;
	uint8_t leg_colour;
	uint8_t skin_colour;
	uint8_t combat_level;
	uint8_t skulled;
	char public_chat_enc[MAX_PUBLIC_CHAT_LEN];
	size_t public_chat_len;
	uint16_t walk_queue_x[WALK_QUEUE_LEN];
	uint16_t walk_queue_y[WALK_QUEUE_LEN];
	uint16_t walk_queue_pos;
	uint16_t walk_queue_len;
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

/* mob.c */
bool mob_within_range(struct mob *, int, int, int);
size_t get_nearby_players(struct mob *, struct player **, size_t, int);

/* player.c */
struct player *player_accept(struct server *, int);
void player_process_walk_queue(struct player *);

/* incoming.c */
int player_parse_incoming(struct player *);

/* outgoing.c */
int player_send_plane_init(struct player *);
int player_send_movement(struct player *);
int player_send_appearance_update(struct player *);

#endif

#ifndef ENTITY_H
#define ENTITY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PLAYER_BUFSIZE		(5000)

#define PLANE_WIDTH		(2304)
#define PLANE_HEIGHT		(1776)
#define PLANE_LEVEL_INC		(944)

#define MAX_KNOWN_PLAYERS	(255)
#define MAX_PUBLIC_CHAT_LEN	(80)
#define MAX_SKILL_ID		(16)
#define MAX_CLIENT_SETTINGS	(3)
#define MAX_INV_SIZE		(30)

/* final client supports 100 ignore and 200 friend entries */
#define MAX_FRIENDS		(200)
#define MAX_IGNORE		(100)

#define WALK_QUEUE_LEN		(16)

struct server;
struct ranctx;

enum skills {
	SKILL_ATTACK		= 0,
	SKILL_DEFENSE		= 1,
	SKILL_STRENGTH		= 2,
	SKILL_HITS		= 3,
	SKILL_RANGED		= 4,
	SKILL_PRAYER		= 5,
};

enum combat_style {
	COMBAT_STYLE_CONTROLLED	= 0,
	COMBAT_STYLE_AGGRESSIVE	= 1,
	COMBAT_STYLE_ACCURATE	= 2,
	COMBAT_STYLE_DEFENSIVE	= 3,
};

/* TODO: expand with a they/them option */
enum mobgender {
	MOB_GENDER_MALE		= 1,
	MOB_GENDER_FEMALE	= 2,
};

enum mobdir {
	MOB_DIR_NORTH		= 0,
	MOB_DIR_NORTHWEST	= 1,
	MOB_DIR_WEST		= 2,
	MOB_DIR_SOUTHWEST	= 3,
	MOB_DIR_SOUTH		= 4,
	MOB_DIR_SOUTHEAST	= 5,
	MOB_DIR_EAST		= 6,
	MOB_DIR_NORTHEAST	= 7,
	/*
	 * attacker is shown on right, defender on left
	 * except NPCs are always on the left
	 */
	MOB_DIR_COMBAT_LEFT	= 8,
	MOB_DIR_COMBAT_RIGHT	= 9
};

struct mob {
	struct server *server;
	uint16_t x, y;
	uint16_t id;
	uint8_t dir;
	uint8_t prev_dir;
	uint8_t moved;
	uint8_t in_combat;
	uint8_t damage;
	uint32_t combat_rounds;
	uint64_t combat_timer;
	uint64_t combat_next_hit;
	uint8_t cur_stats[MAX_SKILL_ID];
	uint8_t base_stats[MAX_SKILL_ID];
	uint8_t combat_level;
	int16_t target_player;
	int16_t target_npc;
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

struct invitem {
	uint16_t id;
	uint32_t stack;
	uint8_t worn;
};

struct player {
	struct mob mob;
	int sock;
	int16_t known_players[MAX_KNOWN_PLAYERS];
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
	int16_t following_player;
	uint8_t stats_changed;
	uint8_t appearance_changed;
	uint8_t plane_changed;
	uint8_t inv_changed;
	uint8_t moved;
	uint8_t sprites[12];
	uint8_t client_settings[MAX_CLIENT_SETTINGS];
	uint8_t gender;
	uint8_t hair_colour;
	uint8_t top_colour;
	uint8_t leg_colour;
	uint8_t skin_colour;
	uint8_t rpg_class;
	uint8_t skulled;
	char public_chat_enc[MAX_PUBLIC_CHAT_LEN];
	size_t public_chat_len;
	uint16_t walk_queue_x[WALK_QUEUE_LEN];
	uint16_t walk_queue_y[WALK_QUEUE_LEN];
	uint16_t walk_queue_pos;
	uint16_t walk_queue_len;
	uint8_t ui_dialog_open;
	uint8_t ui_bank_open;
	uint8_t ui_design_open;
	uint8_t block_public;
	uint8_t block_private;
	uint8_t block_trade;
	uint8_t block_duel;
	uint8_t logout_confirmed;
	uint64_t last_packet;
	uint32_t experience[MAX_SKILL_ID];
	uint8_t quest_points;
	int64_t friend_list[MAX_FRIENDS];
	int64_t ignore_list[MAX_IGNORE];
	uint16_t friend_count;
	uint16_t ignore_count;
	uint8_t combat_style;
	uint8_t weapon_aim;
	uint8_t weapon_power;
	uint8_t armour;
	uint8_t inv_count;
	struct invitem inventory[MAX_INV_SIZE];
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
int mob_combat_roll(struct ranctx *, int, int, int, int, int, int);
int mob_wilderness_level(struct mob *);
size_t get_nearby_players(struct mob *, struct player **, size_t, int);
void mob_combat_reset(struct mob *);

/* player.c */
struct player *player_accept(struct server *, int);
void player_process_walk_queue(struct player *);
void player_process_combat(struct player *);
void player_die(struct player *);
void player_close_ui(struct player *);
void player_destroy(struct player *);
bool player_has_ignore(struct player *, int64_t);
bool player_has_friend(struct player *, int64_t);
bool player_public_chat_visible(struct player *, int64_t);
bool player_is_blocked(struct player *, int64_t, bool);
int player_retreat(struct player *);
int player_add_ignore(struct player *, int64_t);
int player_add_friend(struct player *, int64_t);
int player_remove_ignore(struct player *, int64_t);
int player_remove_friend(struct player *, int64_t);
void player_pvp_attack(struct player *, struct player *);
int player_wear(struct player *, int);
int player_unwear(struct player *, int);

/* incoming.c */
int player_parse_incoming(struct player *);

/* outgoing.c */
int player_send_plane_init(struct player *);
int player_send_movement(struct player *);
int player_send_appearance_update(struct player *);
int player_send_design_ui(struct player *);
int player_send_logout(struct player *);
int player_send_message(struct player *, const char *);
int player_send_stats_update(struct player *);
int player_send_client_settings(struct player *);
int player_send_privacy_settings(struct player *);
int player_send_init_friends(struct player *);
int player_send_init_ignore(struct player *);
int player_send_pm(struct player *, int64_t, uint8_t *, size_t);
int player_send_death(struct player *);
int player_send_inv(struct player *);
int player_send_inv_slot(struct player *, int, int, uint32_t);
int player_send_inv_remove(struct player *, int);
int player_notify_friend_online(struct player *, int64_t);
int player_notify_friend_offline(struct player *, int64_t);

#endif

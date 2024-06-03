#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "opcodes.h"
#include "../buffer.h"
#include "../entity.h"
#include "../server.h"
#include "../zone.h"

#define UPDATE_RADIUS		(15)

#define MAX_NEARBY_LOCS		(256)

enum player_update_type {
	PLAYER_UPDATE_BUBBLE		= 0,
	PLAYER_UPDATE_CHAT		= 1,
	PLAYER_UPDATE_DAMAGE		= 2,
	PLAYER_UPDATE_SHOOT_MONSTER	= 3,
	PLAYER_UPDATE_SHOOT_PLAYER	= 4,
	PLAYER_UPDATE_APPEARANCE	= 5,
};

static ssize_t player_send_appearance(struct player *, void *, size_t);
static ssize_t player_send_damage(struct player *, void *, size_t);
static int player_write_packet(struct player *, void *, size_t);

static int
player_write_packet(struct player *p, void *b, size_t len)
{
	uint8_t *payload = b;
	size_t off = p->outbuf_len;
	size_t orig_off = off;

	if (len >= 160) {
		if (buf_putu8(p->outbuf, off++, PLAYER_BUFSIZE,
			      160 + (len / 256)) == -1) {
			return -1;
		}
		if (buf_putu8(p->outbuf, off++, PLAYER_BUFSIZE,
			      len & 0xff) == -1) {
			return -1;
		}
	} else {
		if (buf_putu8(p->outbuf, off++, PLAYER_BUFSIZE,
			      len & 0xff) == -1) {
			return -1;
		}
		if (len > 0) {
			if (buf_putu8(p->outbuf, off++, PLAYER_BUFSIZE,
			    payload[len - 1]) == -1) {
				return -1;
			}
			len--;
		}
	}
	if (off > (SSIZE_MAX - len) ||
	    ((ptrdiff_t)PLAYER_BUFSIZE - (ptrdiff_t)off) < len) {
		return -1;
	}
	memcpy(p->outbuf + off, b, len);
	off += len;
	p->outbuf_len += (off - orig_off);
	return 0;
}

int
player_send_plane_init(struct player *p)
{
	size_t offset = 0;

	buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		  OP_SRV_INIT_PLANE);

	buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE, p->mob.id);
	offset += 2;

	buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE, PLANE_WIDTH);
	offset += 2;

	buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE, PLANE_HEIGHT);
	offset += 2;

	buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
		  p->mob.y / PLANE_LEVEL_INC);
	offset += 2;

	buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
		  PLANE_LEVEL_INC);
	offset += 2;

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_movement(struct player *p)
{
	size_t offset = 0;
	size_t bitpos;
	struct player *nearby[MAX_KNOWN_PLAYERS];
	int16_t new_known[MAX_KNOWN_PLAYERS];
	size_t nearby_count = 0;
	size_t new_known_count = 0;

	nearby_count = get_nearby_players(&p->mob, nearby,
	    MAX_KNOWN_PLAYERS, UPDATE_RADIUS);

	buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		  OP_SRV_PLAYER_MOVEMENT);

	bitpos = offset * 8;

	buf_putbits(p->tmpbuf, bitpos,
		    PLAYER_BUFSIZE, 10, p->mob.x);
	bitpos += 10;

	buf_putbits(p->tmpbuf, bitpos,
		    PLAYER_BUFSIZE, 12, p->mob.y);
	bitpos += 12;

	buf_putbits(p->tmpbuf, bitpos,
	            PLAYER_BUFSIZE, 4, p->mob.dir);
	bitpos += 4;

	buf_putbits(p->tmpbuf, bitpos,
		    PLAYER_BUFSIZE, 8, p->known_player_count);
	bitpos += 8;

	/* players the client already knows */
	for (size_t i = 0; i < p->known_player_count; ++i) {
		struct player *known_player;

		if (p->known_players[i] == -1) {
			continue;
		}
		known_player = p->mob.server->players[p->known_players[i]];
		if (known_player == NULL ||
		    known_player->logout_confirmed ||
		    !mob_within_range(&known_player->mob,
			p->mob.x, p->mob.y, UPDATE_RADIUS)) {
			if (buf_putbits(p->tmpbuf, bitpos,
					PLAYER_BUFSIZE, 4, 15) == -1) {
				return -1;
			}
			p->known_players[i] = -1;
			bitpos += 4;
		} else if (known_player->moved) {
			/*
			 * TODO: this assertion can be fired!!! it's fatal
			 * because there isn't room in the next bits
			 */
			assert(known_player->mob.dir != MOB_DIR_COMBAT_LEFT &&
			    known_player->mob.dir != MOB_DIR_COMBAT_RIGHT);
			if (buf_putbits(p->tmpbuf, bitpos++,
					PLAYER_BUFSIZE, 1, 1) == -1) {
				return -1;
			}
			if (buf_putbits(p->tmpbuf, bitpos++,
					PLAYER_BUFSIZE, 1, 0) == -1) {
				return -1;
			}
			if (buf_putbits(p->tmpbuf, bitpos,
					PLAYER_BUFSIZE, 3,
					known_player->mob.dir) == -1) {
				return -1;
			}
			bitpos += 3;
		} else if (known_player->mob.dir !=
			    known_player->mob.prev_dir) {
			if (buf_putbits(p->tmpbuf, bitpos++,
					PLAYER_BUFSIZE, 1, 1) == -1) {
				return -1;
			}
			if (buf_putbits(p->tmpbuf, bitpos++,
					PLAYER_BUFSIZE, 1, 1) == -1) {
				return -1;
			}
			if (buf_putbits(p->tmpbuf, bitpos,
					PLAYER_BUFSIZE, 4,
					known_player->mob.dir) == -1) {
				return -1;
			}
			bitpos += 4;
		} else {
			if (buf_putbits(p->tmpbuf, bitpos++,
					PLAYER_BUFSIZE, 1, 0) == -1) {
				return -1;
			}
		}
		if (p->known_players[i] != -1) {
			new_known[new_known_count++] = p->known_players[i];
		}
	}

	memset(p->known_players_seen, 0, new_known_count * sizeof(uint8_t));

	/* players the client doesn't know about yet */
	for (size_t i = 0; i < nearby_count; ++i) {
		if (p->known_player_count >= MAX_KNOWN_PLAYERS) {
			break;
		}
		if (nearby[i] == p || nearby[i]->logout_confirmed) {
			continue;
		}
		bool known = false;
		for (size_t j = 0; j < new_known_count; ++j) {
			if (new_known[j] == nearby[i]->mob.id) {
				known = true;
				break;
			}
		}
		if (known) {
			continue;
		}

		if (buf_putbits(p->tmpbuf, bitpos,
				PLAYER_BUFSIZE, 11, nearby[i]->mob.id) == -1) {
			return -1;
		}
		bitpos += 11;

		if (buf_putbits(p->tmpbuf, bitpos, PLAYER_BUFSIZE, 5,
				(int)nearby[i]->mob.x - (int)p->mob.x) == -1) {
			return -1;
		}
		bitpos += 5;

		if (buf_putbits(p->tmpbuf, bitpos, PLAYER_BUFSIZE, 5,
				(int)nearby[i]->mob.y - (int)p->mob.y) == -1) {
			return -1;
		}
		bitpos += 5;

		if (buf_putbits(p->tmpbuf, bitpos, PLAYER_BUFSIZE, 4,
				nearby[i]->mob.dir) == -1) {
			return -1;
		}
		bitpos += 4;

		if (buf_putbits(p->tmpbuf, bitpos, PLAYER_BUFSIZE, 1, 0) == -1) {
			return -1;
		}
		bitpos++;

		p->known_players_seen[new_known_count] = false;
		new_known[new_known_count++] = nearby[i]->mob.id;
	}

	memcpy(p->known_players, new_known,
	    new_known_count * sizeof(int16_t));
	p->known_player_count = new_known_count;

	offset = (bitpos + 7) / 8;

	return player_write_packet(p, p->tmpbuf, offset);
}

static ssize_t
player_send_appearance(struct player *p, void *tmpbuf, size_t offset)
{
	if (buf_putu16(tmpbuf, offset, PLAYER_BUFSIZE,
		       p->mob.id) == -1) {
		return -1;
	}
	offset += 2;

	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      PLAYER_UPDATE_APPEARANCE) == -1) {
		return -1;
	}

	if (buf_putu16(tmpbuf, offset, PLAYER_BUFSIZE,
		       p->mob.id) == -1) {
		return -1;
	}
	offset += 2;

	if (buf_putu64(tmpbuf, offset, PLAYER_BUFSIZE,
		       p->name) == -1) {
		return -1;
	}
	offset += 8;

	int anim_count = 0;
	for (int i = ANIM_SLOT_BACK; i >= 0; --i) {
		if (p->sprites[i] != 0) {
			anim_count = i + 1;
			break;
		}
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      anim_count) == -1) {
		return -1;
	}

	for (int i = 0; i < anim_count; ++i) {
		if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
			      p->sprites[i]) == -1) {
			return -1;
		}
	}

	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->hair_colour) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->top_colour) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->leg_colour) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->skin_colour) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->mob.combat_level) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->skulled) == -1) {
		return -1;
	}
	return offset;
}

static ssize_t
player_send_damage(struct player *p, void *tmpbuf, size_t offset)
{
	if (buf_putu16(tmpbuf, offset, PLAYER_BUFSIZE,
		       p->mob.id) == -1) {
		return -1;
	}
	offset += 2;
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      PLAYER_UPDATE_DAMAGE) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->mob.damage) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->mob.cur_stats[SKILL_HITS]) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->mob.base_stats[SKILL_HITS]) == -1) {
		return -1;
	}
	return offset;
}

int
player_send_design_ui(struct player *p)
{
	size_t offset = 0;
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_SHOW_DESIGN);

	p->ui_design_open = true;
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_message(struct player *p, const char *s)
{
	size_t offset = 0;
	size_t len = strlen(s);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_MESSAGE);
	if (buf_putdata(p->tmpbuf, offset, PLAYER_BUFSIZE,
			(void *)s, len) != -1) {
		offset += len;
		return player_write_packet(p, p->tmpbuf, offset);
	}
	return -1;
}

int
player_send_logout(struct player *p)
{
	size_t offset = 0;
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_LOGOUT);

	p->logout_confirmed = true;
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_logout_reject(struct player *p)
{
	size_t offset = 0;
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_LOGOUT_REJECT);

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_prayers(struct player *p)
{
	size_t offset = 0;
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_PRAYERS);

	for (int i = 0; i < MAX_PRAYERS; ++i) {
		struct prayer_config *config = server_prayer_config_by_id(i);
		assert(config != NULL);
		if (config->level > p->mob.base_stats[SKILL_PRAYER]) {
			break;
		}
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
				p->prayers[i]) == -1) {
			return -1;
		}
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_client_settings(struct player *p)
{
	size_t offset = 0;
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_CLIENT_SETTINGS);

	for (int i = 0; i < MAX_CLIENT_SETTINGS; ++i) {
		(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
				p->client_settings[i]);
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_privacy_settings(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_PRIVACY_SETTINGS);
	/*
	 * first one is "hide online status", unique to these earlier
	 * revisions, but we treat it identically to private block
	 */
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->block_private);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->block_public);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->block_private);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->block_trade);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->block_duel);
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_stat(struct player *p, int stat)
{
	size_t offset = 0;

	assert(stat < MAX_SKILL_ID);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, OP_SRV_STAT);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, (uint8_t)stat);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
	    p->mob.cur_stats[stat]);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
	    p->mob.base_stats[stat]);
	(void)buf_putu32(p->tmpbuf, offset, PLAYER_BUFSIZE,
	    p->experience[stat]);
	offset += 4;

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_stat_xp(struct player *p, int stat)
{
	size_t offset = 0;

	assert(stat < MAX_SKILL_ID);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, OP_SRV_STAT_XP);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, (uint8_t)stat);
	(void)buf_putu32(p->tmpbuf, offset, PLAYER_BUFSIZE,
	    p->experience[stat]);
	offset += 4;

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_init_stats(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, OP_SRV_INIT_STATS);

	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
				p->mob.cur_stats[i]) == -1) {
			return -1;
		}
	}

	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
				p->mob.base_stats[i]) == -1) {
			return -1;
		}
	}

	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		if (buf_putu32(p->tmpbuf, offset, PLAYER_BUFSIZE,
				p->experience[i]) == -1) {
			return -1;
		}
		offset += 4;
	}

	if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			p->quest_points) == -1) {
		return -1;
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_appearance_update(struct player *p)
{
	uint16_t update_count = 0;
	size_t offset = 0, lenofs = 0;
	ssize_t tmpofs;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_PLAYER_APPEARANCE);

	/* allocate placeholder for length */
	lenofs = offset;
	offset += 2;

	if (p->appearance_changed) {
		tmpofs = player_send_appearance(p, p->tmpbuf, offset);
		if (tmpofs == -1) {
			return -1;
		}
		offset = tmpofs;
		update_count++;
	}
	if (p->mob.damage != UINT8_MAX) {
		tmpofs = player_send_damage(p, p->tmpbuf, offset);
		if (tmpofs == -1) {
			return -1;
		}
		offset = tmpofs;
		update_count++;
	}

	for (int i = 0; i < p->known_player_count; ++i) {
		struct player *p2;

		if (p->known_players[i] == -1) {
			continue;
		}
		p2 = p->mob.server->players[p->known_players[i]];
		if (p2 == NULL || p2->logout_confirmed) {
			continue;
		}
		if (p2->public_chat_len > 0 &&
		    !player_is_blocked(p, p2->name, p->block_public)) {
			if (buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
				       p2->mob.id) == -1) {
				return -1;
			}
			offset += 2;
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
				      PLAYER_UPDATE_CHAT) == -1) {
				return -1;
			}
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
				      p2->public_chat_len) == -1) {
				return -1;
			}
			if (buf_putdata(p->tmpbuf, offset, PLAYER_BUFSIZE,
			      p2->public_chat_enc, p2->public_chat_len) == -1) {
				return -1;
			}
			offset += p2->public_chat_len;
			update_count++;
		}
		if (p2->mob.damage != UINT8_MAX) {
			tmpofs = player_send_damage(p2, p->tmpbuf, offset);
			if (tmpofs == -1) {
				return -1;
			}
			offset = tmpofs;
			update_count++;
		}
		if (p2->appearance_changed || !p->known_players_seen[i]) {
			tmpofs = player_send_appearance(p2, p->tmpbuf, offset);
			if (tmpofs == -1) {
				return -1;
			}
			offset = tmpofs;
			update_count++;
			p->known_players_seen[i] = true;
		}
	}

	if (update_count > 0) {
		(void)buf_putu16(p->tmpbuf, lenofs, PLAYER_BUFSIZE,
				 update_count);
		return player_write_packet(p, p->tmpbuf, offset);
	}
	return 0;
}

int
player_send_init_friends(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_INIT_FRIENDS);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			p->friend_count);

	for (int i = 0; i < p->friend_count; ++i) {
		struct player *p2;

		if (buf_putu64(p->tmpbuf, offset, PLAYER_BUFSIZE,
				p->friend_list[i]) == -1) {
			return -1;
		}
		offset += 8;
		p2 = server_find_player_name37(p->friend_list[i]);
		if (p2 == NULL || player_is_blocked(p2, p->name, p2->block_private)) {
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, 0) == -1) {
				return -1;
			}
		} else {
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, 1) == -1) {
				return -1;
			}
		}
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_init_ignore(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_INIT_IGNORE);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			p->ignore_count);

	for (int i = 0; i < p->ignore_count; ++i) {
		if (buf_putu64(p->tmpbuf, offset, PLAYER_BUFSIZE,
				p->ignore_list[i]) == -1) {
			return -1;
		}
		offset += 8;
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_notify_friend_online(struct player *p, int64_t friend)
{
	size_t offset = 0;
	struct player *p2;

	if (!player_has_friend(p, friend)) {
		return 0;
	}

	p2 = server_find_player_name37(friend);
	if (p2 != NULL && player_is_blocked(p2, p->name, p2->block_private)) {
		return 0;
	}

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_FRIEND_UPDATE);
	(void)buf_putu64(p->tmpbuf, offset, PLAYER_BUFSIZE, friend);
	offset += 8;
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, 1);

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_notify_friend_offline(struct player *p, int64_t friend)
{
	size_t offset = 0;
	struct player *p2;

	if (!player_has_friend(p, friend)) {
		return 0;
	}

	p2 = server_find_player_name37(friend);
	if (p2 != NULL && player_is_blocked(p2, p->name, p2->block_private)) {
		return 0;
	}

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_FRIEND_UPDATE);
	(void)buf_putu64(p->tmpbuf, offset, PLAYER_BUFSIZE, friend);
	offset += 8;
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, 0);

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_pm(struct player *p, int64_t from, uint8_t *encoded, size_t len)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_PRIVATE_MESSAGE);
	(void)buf_putu64(p->tmpbuf, offset, PLAYER_BUFSIZE, from);
	offset += 8;
	if (buf_putdata(p->tmpbuf, offset, PLAYER_BUFSIZE,
			encoded, len) == -1) {
		return -1;
	}
	offset += len;
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_death(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_DEATH);
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_inv(struct player *p)
{
	size_t offset = 0;
	size_t tmpofs;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_INVENTORY);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->inv_count);
	for (int i = 0; i < p->inv_count; ++i) {
		struct item_config *item;
		int id = p->inventory[i].id;

		if (p->inventory[i].worn) {
			id += 0x8000;
		}
		if (buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE, id) == -1) {
			return -1;
		}
		offset += 2;
		item = server_item_config_by_id(p->inventory[i].id);
		if (item == NULL) {
			return -1;
		}
		if (item->weight == 0) {
			tmpofs = buf_putsmartu32(p->tmpbuf, offset,
			    PLAYER_BUFSIZE, p->inventory[i].stack);
			if (tmpofs == -1) {
				return -1;
			}
			offset = tmpofs;
		}
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_inv_slot(struct player *p, int slot, int id, uint32_t stack)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_INVENTORY_ITEM);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, slot);
	(void)buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE, id);
	offset += 2;
	offset = buf_putsmartu32(p->tmpbuf, offset, PLAYER_BUFSIZE, id);
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_inv_remove(struct player *p, int slot)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_INVENTORY_REMOVE);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, slot);
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_equip_bonuses(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_EQUIP_BONUSES);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->bonus_armour);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->bonus_weaponaim);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->bonus_weaponpower);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->bonus_magic);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->bonus_prayer);
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_locs(struct player *p)
{
	size_t offset = 0;
	struct loc nearby[MAX_NEARBY_LOCS];
	size_t nearby_count = 0;
	size_t update_count = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_LOCS);

	for (int i = 0; i < p->known_loc_count; ++i) {
		struct loc *loc;

		loc = server_find_loc(p->known_locs[i].x, p->known_locs[i].y);
		if (loc == NULL) {
			if (buf_putu16(p->tmpbuf, offset,
			    PLAYER_BUFSIZE, 60000) == -1) {
				return -1;
			}
			offset += 2;
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			    (uint8_t)(loc->x - (int)p->mob.x)) == -1) {
				return -1;
			}
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			    (uint8_t)(loc->y - (int)p->mob.y)) == -1) {
				return -1;
			}
			update_count++;
			/* TODO remove from player array */
		} else if (loc->id != p->known_locs[i].id) {
			if (buf_putu16(p->tmpbuf, offset,
			    PLAYER_BUFSIZE, loc->id) == -1) {
				return -1;
			}
			offset += 2;
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			    (uint8_t)(loc->x - (int)p->mob.x)) == -1) {
				return -1;
			}
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			    (uint8_t)(loc->y - (int)p->mob.y)) == -1) {
				return -1;
			}
			p->known_locs[i].id = loc->id;
			update_count++;
		}
	}

	nearby_count = mob_get_nearby_locs(&p->mob,
	    nearby, MAX_NEARBY_LOCS);

	for (size_t i = 0; i < nearby_count; ++i) {
		if (player_has_known_loc(p, nearby[i].x, nearby[i].y)) {
			continue;
		}
		if (buf_putu16(p->tmpbuf, offset,
		    PLAYER_BUFSIZE, nearby[i].id) == -1) {
			return -1;
		}
		offset += 2;
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		    (uint8_t)(nearby[i].x - (int)p->mob.x)) == -1) {
			return -1;
		}
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		    (uint8_t)(nearby[i].y - (int)p->mob.y)) == -1) {
			return -1;
		}
		player_add_known_loc(p, &nearby[i]);
		update_count++;
	}

	if (update_count == 0) {
		/* nothing to inform client */
		return 0;
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "opcodes.h"
#include "../buffer.h"
#include "../entity.h"
#include "../server.h"
#include "../utility.h"
#include "../zone.h"

#define UPDATE_RADIUS		(15)

#define MAX_NEARBY_LOCS		(256)
#define MAX_NEARBY_BOUNDS	(256)
#define MAX_NEARBY_ITEMS	(512)

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
static ssize_t player_send_projectile(struct player *, void *, size_t);
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
	if (off > (size_t)(SSIZE_MAX - len) ||
	    ((ptrdiff_t)PLAYER_BUFSIZE - (ptrdiff_t)off) < (ptrdiff_t)len) {
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
player_send_npc_movement(struct player *p)
{
	size_t offset = 0;
	size_t bitpos;
	struct npc *nearby[MAX_KNOWN_NPCS];
	int16_t new_known[MAX_KNOWN_NPCS];
	size_t nearby_count = 0;
	size_t new_known_count = 0;

	nearby_count = get_nearby_npcs(&p->mob, nearby,
	    MAX_KNOWN_NPCS, UPDATE_RADIUS);

	buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		  OP_SRV_NPC_MOVEMENT);

	buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		  p->known_npc_count);

	bitpos = offset * 8;

	/* NPCs the client already knows */
	for (size_t i = 0; i < p->known_npc_count; ++i) {
		struct npc *known_npc;

		if (p->known_npcs[i] == -1) {
			continue;
		}
		known_npc = p->mob.server->npcs[p->known_npcs[i]];
		if (known_npc == NULL ||
		    !mob_within_range(&known_npc->mob,
			p->mob.x, p->mob.y, UPDATE_RADIUS)) {
			if (buf_putbits(p->tmpbuf, bitpos,
					PLAYER_BUFSIZE, 4, 15) == -1) {
				return -1;
			}
			p->known_npcs[i] = -1;
			bitpos += 4;
		} else if (known_npc->mob.moved) {
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
					known_npc->mob.dir) == -1) {
				return -1;
			}
			bitpos += 3;
		} else if (known_npc->mob.dir !=
			    known_npc->mob.prev_dir) {
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
					known_npc->mob.dir) == -1) {
				return -1;
			}
			bitpos += 4;
		} else {
			if (buf_putbits(p->tmpbuf, bitpos++,
					PLAYER_BUFSIZE, 1, 0) == -1) {
				return -1;
			}
		}
		if (p->known_npcs[i] != -1) {
			new_known[new_known_count++] = p->known_npcs[i];
		}
	}

	/* NPCs the client doesn't know about yet */
	for (size_t i = 0; i < nearby_count; ++i) {
		if (p->known_npc_count >= MAX_KNOWN_NPCS) {
			break;
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

		if (buf_putbits(p->tmpbuf, bitpos, PLAYER_BUFSIZE, 8,
				nearby[i]->config->id) == -1) {
			return -1;
		}
		bitpos += 8;

		new_known[new_known_count++] = nearby[i]->mob.id;
	}

	memcpy(p->known_npcs, new_known,
	    new_known_count * sizeof(int16_t));
	p->known_npc_count = new_known_count;

	offset = (bitpos + 7) / 8;

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
			p->known_players_seen[new_known_count] = true;
			new_known[new_known_count++] = p->known_players[i];
		}
	}

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

static ssize_t
player_send_projectile(struct player *p, void *tmpbuf, size_t offset)
{
	if (buf_putu16(tmpbuf, offset, PLAYER_BUFSIZE,
		       p->mob.id) == -1) {
		return -1;
	}
	offset += 2;
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      PLAYER_UPDATE_SHOOT_PLAYER) == -1) {
		return -1;
	}
	if (buf_putu16(tmpbuf, offset, PLAYER_BUFSIZE,
		      p->projectile_sprite) == -1) {
		return -1;
	}
	offset += 2;
	if (buf_putu16(tmpbuf, offset, PLAYER_BUFSIZE,
		      p->projectile_target_player) == -1) {
		return -1;
	}
	offset += 2;
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
	if (p->projectile_sprite != UINT16_MAX) {
		tmpofs = player_send_projectile(p, p->tmpbuf, offset);
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
		if (p2->projectile_sprite != UINT16_MAX) {
			tmpofs = player_send_projectile(p2, p->tmpbuf, offset);
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
	ssize_t tmpofs;

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
player_send_inv_slot(struct player *p, int slot)
{
	size_t offset = 0;
	struct item_config *config;
	bool stackable = false;
	int id = p->inventory[slot].id;

	config = server_item_config_by_id(p->inventory[slot].id);
	if (config != NULL && config->weight == 0) {
		stackable = true;
	}
	if (p->inventory[slot].worn) {
		id += 0x8000;
	}

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_INVENTORY_ITEM);
	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, slot);
	(void)buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE, id);
	offset += 2;
	if (stackable) {
		offset = buf_putsmartu32(p->tmpbuf, offset,
		    PLAYER_BUFSIZE, p->inventory[slot].stack);
	}
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
	size_t removed_count = 0;
	struct zone *player_zone;
	struct zone *zone;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_LOCS);

	player_zone = server_find_zone(p->mob.x, p->mob.y);

	for (size_t i = 0; i < p->known_loc_count; ++i) {
		struct loc *loc;
		bool remove = false;

		loc = server_find_loc(p->known_locs[i].x, p->known_locs[i].y);
		if (loc == NULL) {
			remove = true;
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
		} else {
			/*
			 * remove locs far beyond the update range to free
			 * up client and server memory, but don't remove
			 * those on a nearby plane in case the player
			 * travels up or down stairs
			 */
			zone = server_find_zone(p->known_locs[i].x,
			    p->known_locs[i].y);
			if (zone != NULL && player_zone != NULL) {
				if (abs(zone->x - (int)player_zone->x) >= 8 ||
				    abs(zone->y - (int)player_zone->y) >= 8) {
					remove = true;
				}
			} else {
				remove = true;
			}
		}
		if (remove) {
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
			removed_count++;
			player_remove_known_loc(p, i);
		}
	}

#if 0 /* just for testing efficiency */
	if (removed_count > 0) {
		printf("Removed %zu locs from player cache\n",
		    removed_count);
	}
	printf("Player loc cache size: %zu\n", p->known_loc_count);
#else
	(void)removed_count;
#endif

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

int
player_send_bounds(struct player *p)
{
	size_t offset = 0;
	struct bound nearby[MAX_NEARBY_BOUNDS];
	size_t nearby_count = 0;
	size_t update_count = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_BOUNDARIES);

	for (size_t i = 0; i < p->known_bound_count; ++i) {
		struct bound *bound;

		bound = server_find_bound(p->known_bounds[i].x,
		    p->known_bounds[i].y, p->known_bounds[i].dir);
		if (bound == NULL) {
			/*
			 * no way for client to remove individual bounds
			 * due to a bug
			 */
			continue;
		} else if (bound->id != p->known_bounds[i].id) {
			if (buf_putu16(p->tmpbuf, offset,
			    PLAYER_BUFSIZE, bound->id) == -1) {
				return -1;
			}
			offset += 2;
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			    (uint8_t)(bound->x - (int)p->mob.x)) == -1) {
				return -1;
			}
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			    (uint8_t)(bound->y - (int)p->mob.y)) == -1) {
				return -1;
			}
			if (buf_putu8(p->tmpbuf, offset++,
			    PLAYER_BUFSIZE, bound->dir) == -1) {
				return -1;
			}
			update_count++;
		}
	}

	nearby_count = mob_get_nearby_bounds(&p->mob,
	    nearby, MAX_NEARBY_BOUNDS);

	for (size_t i = 0; i < nearby_count; ++i) {
		if (player_has_known_bound(p,
		    nearby[i].x, nearby[i].y, nearby[i].dir)) {
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
		if (buf_putu8(p->tmpbuf, offset++,
		    PLAYER_BUFSIZE, nearby[i].dir) == -1) {
			return -1;
		}
		player_add_known_bound(p, &nearby[i]);
		update_count++;
	}

	if (update_count == 0) {
		/* nothing to inform client */
		return 0;
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_ground_items(struct player *p)
{
	size_t offset = 0;
	struct ground_item nearby[MAX_NEARBY_ITEMS];
	struct ground_item new_known[MAX_NEARBY_ITEMS];
	struct ground_item *item;
	struct ground_item *global_item;
	size_t nearby_count = 0;
	size_t new_known_count = 0;
	size_t update_count = 0;
	struct zone *origin;
	struct zone *zone;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_GROUND_ITEMS);

	origin = server_find_zone(p->mob.x, p->mob.y);
	assert(origin != NULL);

	for (size_t i = 0; i < p->known_item_count; ++i) {
		bool out_of_range = false;

		global_item = server_find_ground_item(p, p->known_items[i].x,
		    p->known_items[i].y, p->known_items[i].id);
		item = &p->known_items[i];
		zone = server_find_zone(item->x, item->y);
		if (zone != NULL) {
			if (abs(zone->x - (int)origin->x) > 3 ||
			    abs(zone->y - (int)origin->y) > 3) {
				out_of_range = true;
			}
		} else {
			out_of_range = true;
		}
		if (global_item == NULL || out_of_range ||
		    !player_can_see_item(p, item)) {
			/* remove the item from the player's view */
			if (buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
					item->id | 0x8000) == -1) {
				return -1;
			}
			offset += 2;
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			    (uint8_t)(item->x - (int)p->mob.x)) == -1) {
				return -1;
			}
			if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
			    (uint8_t)(item->y - (int)p->mob.y)) == -1) {
				return -1;
			}
			update_count++;
		} else {
			new_known[new_known_count++] = *item;
		}
	}

	nearby_count = player_get_nearby_items(p, nearby, MAX_NEARBY_ITEMS);
	for (size_t i = 0; i < nearby_count; ++i) {
		item = &nearby[i];
		if (player_has_known_item(p, item->unique_id)) {
			continue;
		}
		/* add the item to the player's view */
		if (buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
				item->id) == -1) {
			return -1;
		}
		offset += 2;
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		    (uint8_t)(item->x - (int)p->mob.x)) == -1) {
			return -1;
		}
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		    (uint8_t)(item->y - (int)p->mob.y)) == -1) {
			return -1;
		}
		new_known[new_known_count++] = nearby[i];
		update_count++;
	}

	if (p->known_item_max < new_known_count) {
		size_t n = new_known_count * 2;
		if (n < 64) {
			n = 64;
		}
		if (reallocarr(&p->known_items, n,
		    sizeof(struct ground_item)) == -1) {
			return -1;
		}
		p->known_item_max = n;
	}

	printf("known items %zu new count %zu nearby %zu updates  %zu\n",
	    p->known_item_count, new_known_count, nearby_count, update_count);

	memcpy(p->known_items, new_known,
	    new_known_count * sizeof(struct ground_item));
	p->known_item_count = new_known_count;

	if (update_count == 0) {
		/* nothing to inform client */
		return 0;
	}
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_trade_open(struct player *p)
{
	size_t offset = 0;

	assert(p->trading_player != -1);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_SHOW_TRADE);

	(void)buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
			 p->trading_player);
	offset += 2;

	p->ui_trade_open = true;
	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_partner_trade_offer(struct player *p)
{
	size_t offset = 0;
	struct player *partner;

	assert(p->trading_player != -1);
	partner = p->mob.server->players[p->trading_player];
	assert(partner != NULL);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_UPDATE_TRADE_OFFER);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        partner->offer_count);

	for (int i = 0; i < partner->offer_count; ++i) {
		if (buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
				partner->trade_offer[i].id) == -1) {
			return -1;
		}
		offset += 2;
		if (buf_putu32(p->tmpbuf, offset, PLAYER_BUFSIZE,
				partner->trade_offer[i].stack) == -1) {
			return -1;
		}
		offset += 4;
	}

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_close_trade(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_CLOSE_TRADE);

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_trade_state(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_TRADE_STATE_LOCAL);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->trade_state != TRADE_STATE_NONE);

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_trade_state_remote(struct player *p)
{
	size_t offset = 0;
	struct player *partner;

	assert(p->trading_player != -1);
	partner = p->mob.server->players[p->trading_player];
	assert(partner != NULL);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_TRADE_STATE_REMOTE);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        partner->trade_state != TRADE_STATE_NONE);

	return player_write_packet(p, p->tmpbuf, offset);
}

int
player_send_trade_confirm(struct player *p)
{
	size_t offset = 0;
	struct player *partner;

	assert(p->trading_player != -1);
	partner = p->mob.server->players[p->trading_player];
	assert(partner != NULL);

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        OP_SRV_TRADE_CONFIRM);

	(void)buf_putu64(p->tmpbuf, offset, PLAYER_BUFSIZE,
		         partner->name);
	offset += 8;

	/*
	 * send over the final offer, note that the 110 client
	 * appears to have a bug where this isn't actually
	 * used but rather it displays the previously sent
	 * offer.
	 */

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        p->offer_count);

	for (int i = 0; i < p->offer_count; ++i) {
		if (buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
				p->trade_offer[i].id) == -1) {
			return -1;
		}
		offset += 2;
		if (buf_putu32(p->tmpbuf, offset, PLAYER_BUFSIZE,
				p->trade_offer[i].stack) == -1) {
			return -1;
		}
		offset += 4;
	}

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
		        partner->offer_count);

	for (int i = 0; i < partner->offer_count; ++i) {
		if (buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE,
				partner->trade_offer[i].id) == -1) {
			return -1;
		}
		offset += 2;
		if (buf_putu32(p->tmpbuf, offset, PLAYER_BUFSIZE,
				partner->trade_offer[i].stack) == -1) {
			return -1;
		}
		offset += 4;
	}

	return player_write_packet(p, p->tmpbuf, offset);
}

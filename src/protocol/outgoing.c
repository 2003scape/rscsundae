#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "opcodes.h"
#include "../buffer.h"
#include "../entity.h"

#define UPDATE_RADIUS	(32)

enum player_update_type {
	PLAYER_UPDATE_BUBBLE		= 0,
	PLAYER_UPDATE_CHAT		= 1,
	PLAYER_UPDATE_DAMAGE		= 2,
	PLAYER_UPDATE_SHOOT_MONSTER	= 3,
	PLAYER_UPDATE_SHOOT_PLAYER	= 4,
	PLAYER_UPDATE_APPEARANCE	= 5,
};

static ssize_t player_send_appearance(struct player *, void *, size_t);
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

	buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE, 2304);
	offset += 2;

	buf_putu16(p->tmpbuf, offset, PLAYER_BUFSIZE, 1776);
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
	struct player *new_known[MAX_KNOWN_PLAYERS];
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
		if (p->known_players[i] == NULL) {
			continue;
		}
		if (p->known_players[i]->moved) {
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
					p->known_players[i]->mob.dir) == -1) {
				return -1;
			}
			bitpos += 3;
		} else if (p->known_players[i]->mob.dir !=
			    p->known_players[i]->mob.prev_dir) {
			if (buf_putbits(p->tmpbuf, bitpos,
					PLAYER_BUFSIZE, 3,
					p->known_players[i]->mob.dir) == -1) {
				return -1;
			}
			bitpos += 3;
		} else if (p->known_players[i]->logout_confirmed) {
			if (buf_putbits(p->tmpbuf, bitpos,
					PLAYER_BUFSIZE, 4, 12) == -1) {
				return -1;
			}
			p->known_players[i]->mob.refcount--;
			p->known_players[i] = NULL;
			bitpos += 4;
		} else {
			if (buf_putbits(p->tmpbuf, bitpos++,
					PLAYER_BUFSIZE, 1, 0) == -1) {
				return -1;
			}
		}
		if (p->known_players[i] != NULL) {
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
			if (new_known[j] == nearby[i]) {
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

		p->mob.refcount++;
		p->known_players_seen[new_known_count] = false;
		new_known[new_known_count++] = nearby[i];
	}

	memcpy(p->known_players, new_known,
	    new_known_count * sizeof(struct player *));
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
	for (int i = ANIM_SLOT_CAPE; i >= 0; --i) {
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
		      p->combat_level) == -1) {
		return -1;
	}
	if (buf_putu8(tmpbuf, offset++, PLAYER_BUFSIZE,
		      p->skulled) == -1) {
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
player_send_stats_update(struct player *p)
{
	size_t offset = 0;

	(void)buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE, OP_SRV_STATS);

	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
				p->cur_stats[i]) == -1) {
			return -1;
		}
	}

	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		if (buf_putu8(p->tmpbuf, offset++, PLAYER_BUFSIZE,
				p->base_stats[i]) == -1) {
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

	for (int i = 0; i < p->known_player_count; ++i) {
		struct player *p2 = p->known_players[i];
		if (p2 == NULL || p2->logout_confirmed) {
			continue;
		}
		if (p2->public_chat_len > 0) {
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

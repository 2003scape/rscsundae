#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "opcodes.h"
#include "../buffer.h"
#include "../entity.h"

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
		    PLAYER_BUFSIZE, 8, 0);
	bitpos += 8;

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
		if (p2->appearance_changed || !p->known_players_seen[i]) {
			tmpofs = player_send_appearance(p2, p->tmpbuf, offset);
			if (tmpofs == -1) {
				return -1;
			}
			offset = tmpofs;
			update_count++;
		}
	}

	if (update_count > 0) {
		(void)buf_putu16(p->tmpbuf, lenofs, PLAYER_BUFSIZE,
				 update_count);
		return player_write_packet(p, p->tmpbuf, offset);
	}
	return 0;
}

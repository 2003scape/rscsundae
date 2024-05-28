#include <sys/socket.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocol/opcodes.h"
#include "server.h"
#include "entity.h"
#include "loop.h"

#define ANIM_HEAD1	(0)
#define ANIM_BODY1	(1)
#define ANIM_LEGS1	(2)
#define ANIM_FHEAD1	(3)
#define ANIM_FBODY1	(4)
#define ANIM_HEAD2	(5)
#define ANIM_HEAD3	(6)
#define ANIM_HEAD4	(7)

struct player *
player_accept(struct server *s, int sock)
{
	int slot = -1;
	int flags;
	uint8_t resp[2];

	flags = fcntl(sock, F_GETFL, 0);
	if (flags == -1) {
		printf("get flags failed\n");
		return NULL;
	}

	(void)fcntl(sock, F_SETFL, flags | O_NONBLOCK);

	int32_t session_id = ranval(&s->ran);

	/* this has to happen before a slot is selected. */

	(void)send(sock, &session_id, sizeof(session_id), 0);

	/* try to find space for the player */

	for (int i = 0; i < MAXPLAYERS; ++i) {
		if (s->players[i] == NULL) {
			slot = i;
			break;
		}
	}

	if (slot >= s->max_player_id) {
		s->max_player_id = slot + 1;
		printf("set max player id %d\n", s->max_player_id);
	}

	if (slot == -1) {
		/* no space */
		printf("server is full\n");
		resp[0] = 0;
		resp[1] = RESP_FULL;
		(void)send(sock, &resp, sizeof(resp), 0);
		return NULL;
	}

	struct player *p = calloc(1, sizeof(struct player));
	if (p == NULL) {
		printf("player alloc failed\n");
		resp[0] = 0;
		resp[1] = RESP_FULL;
		(void)send(sock, &resp, sizeof(resp), 0);
		return NULL;
	}

	resp[0] = 0;
	resp[1] = 0;
	(void)send(sock, &resp, sizeof(resp), 0);

	p->name = -1;
	p->mob.id = (uint16_t)slot;
	p->session_id = session_id;
	p->sock = sock;

	p->sprites[ANIM_SLOT_HEAD] = ANIM_FHEAD1 + 1;
	p->sprites[ANIM_SLOT_BODY] = ANIM_FBODY1 + 1;
	p->sprites[ANIM_SLOT_LEGS] = ANIM_LEGS1 + 1;
	p->combat_level = 3;
	p->appearance_changed = 1;
	p->plane_changed = 1;

	p->mob.server = s;
	p->mob.x = 120;
	p->mob.y = 648;
	s->players[slot] = p;
	loop_add_player(p);
	return p;
}

void
player_process_walk_queue(struct player *p)
{
	int pos = p->walk_queue_pos;
	int remaining = p->walk_queue_len - pos;
	if (remaining == 0) {
		p->walk_queue_pos = 0;
		p->walk_queue_len = 0;
		return;
	}
	int cur_x = p->mob.x;
	int cur_y = p->mob.y;
	if (cur_x == p->walk_queue_x[pos] &&
	    cur_y == p->walk_queue_y[pos]) {
		p->walk_queue_pos = 0;
		p->walk_queue_len = 0;
		return;
	}
	int dif_x = cur_x - (int)p->walk_queue_x[pos];
	int dif_y = cur_y - (int)p->walk_queue_y[pos];

	if (dif_x == 0) {
		if (dif_y > 0) {
			p->mob.dir = MOB_DIR_NORTH;
			p->mob.y = cur_y - 1;
		} else if (dif_y < 0) {
			p->mob.dir = MOB_DIR_SOUTH;
			p->mob.y = cur_y + 1;
		}
	} else if (dif_x < 0) {
		if (dif_y == 0) {
			p->mob.dir = MOB_DIR_WEST;
			p->mob.x = cur_x + 1;
		} else if (dif_y < 0) {
			p->mob.dir = MOB_DIR_SOUTHWEST;
			p->mob.x = cur_x + 1;
			p->mob.y = cur_y + 1;
		} else if (dif_y > 0) {
			p->mob.dir = MOB_DIR_NORTHWEST;
			p->mob.x = cur_x + 1;
			p->mob.y = cur_y - 1;
		}
	} else if (dif_x > 0) {
		if (dif_y == 0) {
			p->mob.dir = MOB_DIR_EAST;
			p->mob.x = cur_x - 1;
		} else if (dif_y < 0) {
			p->mob.dir = MOB_DIR_SOUTHEAST;
			p->mob.x = cur_x - 1;
			p->mob.y = cur_y + 1;
		} else if (dif_y > 0) {
			p->mob.dir = MOB_DIR_NORTHEAST;
			p->mob.x = cur_x - 1;
			p->mob.y = cur_y - 1;
		}
	}

	if (p->mob.x != cur_x || p->mob.y != cur_y) {
		p->moved = true;
	}

	p->walk_queue_pos++;
}

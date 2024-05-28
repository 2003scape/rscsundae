#include <sys/socket.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "config/anim.h"
#include "protocol/opcodes.h"
#include "server.h"
#include "entity.h"
#include "loop.h"

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

	for (int i = 0; i < MAX_SKILL_ID; ++i) {
		p->cur_stats[i] = 1;
		p->base_stats[i] = 1;
	}
	p->cur_stats[SKILL_HITS] = 10;
	p->base_stats[SKILL_HITS] = 10;
	p->experience[SKILL_HITS] = 4000;
	p->sprites[ANIM_SLOT_HEAD] = ANIM_FHEAD1 + 1;
	p->sprites[ANIM_SLOT_BODY] = ANIM_FBODY1 + 1;
	p->sprites[ANIM_SLOT_LEGS] = ANIM_LEGS1 + 1;
	p->combat_level = 3;
	p->stats_changed = true;
	p->appearance_changed = true;
	p->plane_changed = true;
	p->ui_design_open = true;
	p->following_player = -1;
	p->last_packet = s->tick_counter;

	p->mob.server = s;
	p->mob.x = 120;
	p->mob.y = 648;
	s->players[slot] = p;

	player_send_design_ui(p);
	player_send_message(p, "@que@Welcome to RSCSundae!");
	loop_add_player(p);
	return p;
}

void
player_process_walk_queue(struct player *p)
{
	if (p->following_player != -1) {
		struct player *p2;

		p2 = p->mob.server->players[p->following_player];
		if (p2 != NULL) {
			if (mob_within_range(&p->mob, p2->mob.x, p2->mob.y, 2)) {
				return;
			}
			p->walk_queue_x[0] = p2->mob.x;
			p->walk_queue_y[0] = p2->mob.y;
			p->walk_queue_pos = 0;
			p->walk_queue_len = 1;
		} else {
			p->following_player = -1;
		}
	}
	int pos = p->walk_queue_pos;
	int remaining = p->walk_queue_len - pos;
	if (remaining == 0) {
		p->walk_queue_pos = 0;
		p->walk_queue_len = 0;
		return;
	}
	int cur_x = p->mob.x;
	int cur_y = p->mob.y;
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

	if (p->mob.x == p->walk_queue_x[pos] &&
	    p->mob.y == p->walk_queue_y[pos]) {
		p->walk_queue_pos++;
	}

	if (p->mob.x != cur_x || p->mob.y != cur_y) {
		player_close_ui(p);
		p->moved = true;
	}

}

void
player_close_ui(struct player *p)
{
	p->ui_dialog_open = false;
	p->ui_design_open = false;
	p->ui_bank_open = false;
}

void
player_destroy(struct player *p) {
	for (int i = 0; i < p->known_player_count; ++i) {
		p->known_players[i]->mob.refcount--;
	}
	if (p->sock != -1) {
		close(p->sock);
		p->sock = -1;
	}
	free(p);
}

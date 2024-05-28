#include <sys/socket.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
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

	p->mob.id = (uint16_t)slot;
	p->session_id = session_id;
	p->sock = sock;
	p->mob.x = 120;
	p->mob.y = 648;
	s->players[slot] = p;
	player_send_plane_init(p);
	loop_add_player(p);
	return p;
}

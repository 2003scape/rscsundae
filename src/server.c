#include "protocol/utility.h"
#include "server.h"
#include "loop.h"
#include "netio.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* XXX no idea what's authentic here */
#define MAX_IDLE_TICKS		(20)

struct server s = {0};

int
main(int argc, char **argv)
{
	/* init random number generator */
	raninit(&s.ran, time(NULL)); 

	/* TODO: port should be configurable */
	if (loop_start(&s, 43594) == -1) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void
server_tick(void)
{
	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		if (s.tick_counter > (s.players[i]->last_packet + MAX_IDLE_TICKS)) {
			player_send_logout(s.players[i]);
		}
		if (s.players[i]->logout_confirmed &&
		    s.players[i]->mob.refcount == 0) {
			char name[32];

			mod37_namedec(s.players[i]->name, name);
			printf("removed player %s\n", name);
			player_destroy(s.players[i]);
			s.players[i] = NULL;
			continue;
		}
		net_player_recv(s.players[i]);
		player_parse_incoming(s.players[i]);
		player_process_walk_queue(s.players[i]);
	}

	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		if (s.players[i]->plane_changed) {
			player_send_plane_init(s.players[i]);
		}
		player_send_movement(s.players[i]);
		player_send_appearance_update(s.players[i]);
		net_player_send(s.players[i]);
	}

	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		s.players[i]->public_chat_len = 0;
		s.players[i]->appearance_changed = false;
		s.players[i]->plane_changed = false;
		s.players[i]->moved = false;
		s.players[i]->mob.prev_dir = s.players[i]->mob.dir;
	}

	s.tick_counter++;
}

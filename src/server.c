#include "server.h"
#include "loop.h"
#include "netio.h"
#include <stdlib.h>
#include <time.h>

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
		net_player_recv(s.players[i]);
		player_parse_incoming(s.players[i]);
		if (s.players[i]->plane_changed) {
			player_send_plane_init(s.players[i]);
			s.players[i]->plane_changed = 0;
		}
		player_send_movement(s.players[i]);
		player_send_appearance_update(s.players[i]);
		net_player_send(s.players[i]);
	}

	for (int i = 0; i < s.max_player_id; ++i) {
		s.players[i]->appearance_changed = 0;
	}
}

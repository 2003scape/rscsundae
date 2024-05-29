#include "protocol/utility.h"
#include "server.h"
#include "loop.h"
#include "netio.h"
#include "entity.h"
#include <sys/types.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* XXX no idea what's authentic here */
#define MAX_IDLE_TICKS		(20)

struct server s = {0};

static void on_signal_do_nothing(int);

int
main(int argc, char **argv)
{
	(void)signal(SIGPIPE, on_signal_do_nothing);

	/* init random number generator */
	raninit(&s.ran, time(NULL));

	/* TODO: port should be configurable */
	if (loop_start(&s, 43594) == -1) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

static void
on_signal_do_nothing(int dummy)
{
	(void)dummy;
}

struct player *
server_find_player_name37(int64_t name)
{
	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i]->name == name) {
			return s.players[i];
		}
	}
	return NULL;
}

void
server_register_login(int64_t name)
{
	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		for (int j = 0; j < s.players[i]->friend_count; ++j) {
			if (s.players[i]->friend_list[j] != name) {
				continue;
			}
			player_notify_friend_online(s.players[i], name);
		}
	}
}

bool
server_has_player(int64_t name)
{
	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		if (s.players[i]->name == name) {
			return true;
		}
	}
	return false;
}

void
server_register_logout(int64_t name)
{
	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		for (int j = 0; j < s.players[i]->friend_count; ++j) {
			if (s.players[i]->friend_list[j] != name) {
				continue;
			}
			player_notify_friend_offline(s.players[i], name);
		}
	}
}

void
server_register_hide_status(struct player *p)
{
	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		for (int j = 0; j < s.players[i]->friend_count; ++j) {
			if (s.players[i]->friend_list[j] != p->name) {
				continue;
			}
			if (player_has_friend(p, s.players[i]->name)) {
				continue;
			}
			player_notify_friend_offline(s.players[i], p->name);
		}
	}
}

void
server_register_unhide_status(struct player *p)
{
	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		for (int j = 0; j < s.players[i]->friend_count; ++j) {
			if (s.players[i]->friend_list[j] != p->name) {
				continue;
			}
			if (player_has_friend(p, s.players[i]->name)) {
				continue;
			}
			player_notify_friend_online(s.players[i], p->name);
		}
	}
}

void
server_send_pm(struct player *from, int64_t target, uint8_t *msg, size_t len)
{
	struct player *targetp;

	targetp = server_find_player_name37(target);
	if (targetp == NULL) {
		return;
	}
	if (player_is_blocked(targetp, from->name, targetp->block_private)) {
		return;
	}
	player_send_pm(targetp, from->name, msg, len);
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
		if (s.players[i]->logout_confirmed) {
			char name[32];

			server_register_logout(s.players[i]->name);
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
		if (s.players[i]->stats_changed) {
			player_send_stats_update(s.players[i]);
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
		s.players[i]->stats_changed = false;
		s.players[i]->moved = false;
		s.players[i]->mob.prev_dir = s.players[i]->mob.dir;
	}

	s.tick_counter++;
}

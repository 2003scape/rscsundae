#include <jag.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "protocol/utility.h"
#include "server.h"
#include "loop.h"
#include "netio.h"
#include "entity.h"
#include "stat.h"
#include "zone.h"

/* XXX no idea what's authentic here */
#define MAX_IDLE_TICKS		(20)

struct server s = {0};

static void on_signal_do_nothing(int);
static int load_config_jag(void);

int
main(int argc, char **argv)
{
	struct loc l;

	s.next_restore = 0;
	s.next_rapid_restore = 0;
	s.next_prayer_drain = 0;
	/* TODO: should be configurable somehow */
	s.xp_multiplier = 1;

	l.x = 330;
	l.y = 333;
	l.id = 20;
	server_add_loc(&l);

	l.x = 326;
	l.y = 333;
	l.id = 40;
	server_add_loc(&l);

	l.x = 328;
	l.y = 333;
	l.id = 60;
	server_add_loc(&l);

	(void)signal(SIGPIPE, on_signal_do_nothing);

	/* init random number generator */
	raninit(&s.ran, time(NULL));

	stat_calculate_table();

	if (load_config_jag() == -1) {
		fprintf(stderr, "error reading config.jag\n");
		fprintf(stderr, "did you run the fetch-jag-files script from the data directory?\n");
		return EXIT_FAILURE;
	}

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
	bool drain_tick = s.tick_counter >= s.next_prayer_drain;
	bool restore_tick = s.tick_counter >= s.next_restore;
	bool rapid_restore_tick = s.tick_counter >= s.next_rapid_restore;

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
		player_process_combat(s.players[i]);
		player_process_walk_queue(s.players[i]);
		if (drain_tick) {
			player_prayer_drain(s.players[i]);
		}
		if (restore_tick) {
			player_slow_restore(s.players[i]);
		}
		if (rapid_restore_tick) {
			player_rapid_restore(s.players[i]);
		}

		if (s.players[i]->skulled &&
		    s.tick_counter > s.players[i]->skull_timer) {
			s.players[i]->skulled = false;
			s.players[i]->appearance_changed = true;
		}
	}

	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		if (s.players[i]->plane_changed) {
			player_send_plane_init(s.players[i]);
		}
		if (s.players[i]->stats_changed) {
			player_send_init_stats(s.players[i]);
		}
		if (s.players[i]->inv_changed) {
			player_send_inv(s.players[i]);
		}
		if (s.players[i]->bonus_changed) {
			player_send_equip_bonuses(s.players[i]);
		}
		if (s.players[i]->prayers_changed) {
			player_send_prayers(s.players[i]);
		}
		player_send_movement(s.players[i]);
		player_send_appearance_update(s.players[i]);
		player_send_locs(s.players[i]);
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
		s.players[i]->inv_changed = false;
		s.players[i]->bonus_changed = false;
		s.players[i]->prayers_changed = false;
		s.players[i]->moved = false;
		s.players[i]->mob.damage = UINT8_MAX;
		s.players[i]->mob.prev_dir = s.players[i]->mob.dir;
	}

	s.tick_counter++;

	if (drain_tick) {
		s.next_prayer_drain = s.tick_counter + 3;
	}

	if (restore_tick) {
		s.next_restore = s.tick_counter + 100;
	}

	if (rapid_restore_tick) {
		s.next_rapid_restore = s.tick_counter + 50;
	}
}

static int
load_config_jag(void)
{
	struct jag_archive archive = {0};
	struct jag_entry entry = {0};
	FILE *f = NULL;

	/* TODO: support configurable & system-wide paths */
	f = fopen("./data/config46.jag", "rb");
	if (f == NULL) {
		goto err;
	}
	if (jag_unpack_file(f, &archive) == -1) {
		goto err;
	}
	fclose(f);
	f = NULL;

	/*
	 * DO NOT free the unpacked entry data unless erroring,
	 * we are reusing it
	 */

	if (jag_find_entry(&archive, "entity.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.entity_config = config_parse_entity((char *)entry.data,
	    entry.unpacked_len, &s.entity_config_count);
	if (s.entity_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu mob sprites (\"entities\")\n",
	    s.entity_config_count);

	if (jag_find_entry(&archive, "objects.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.item_config = config_parse_items((char *)entry.data,
	    entry.unpacked_len, &s.item_config_count,
	    s.entity_config, s.entity_config_count);
	if (s.item_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu items (\"objects\")\n",
	    s.item_config_count);

	if (jag_find_entry(&archive, "prayers.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.prayer_config = config_parse_prayers((char *)entry.data,
	    entry.unpacked_len, &s.prayer_config_count);
	if (s.prayer_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu prayers\n",
	    s.prayer_config_count);

	if (archive.must_free) {
		free(archive.data);
		archive.data = NULL;
	}
	return 0;
err:
	if (entry.must_free) {
		free(entry.data);
		entry.data = NULL;
	}
	if (archive.must_free) {
		free(archive.data);
		archive.data = NULL;
	}
	if (f != NULL) {
		fclose(f);
		f = NULL;
	}
	return -1;
}

struct item_config *
server_item_config_by_id(int id)
{
	if (id < 0 || id >= (int)s.item_config_count) {
		return NULL;
	}
	return &s.item_config[id];
}

struct prayer_config *
server_prayer_config_by_id(int id)
{
	if (id < 0 || id >= (int)s.prayer_config_count) {
		return NULL;
	}
	return &s.prayer_config[id];
}

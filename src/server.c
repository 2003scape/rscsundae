#include <jag.h>
#include <map.h>
#include <ini.h>
#ifdef PROFILE
#include <gperftools/profiler.h>
#endif
#include <sys/types.h>
#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "config/loc.h"
#include "protocol/opcodes.h"
#include "server.h"
#include "loop.h"
#include "netio.h"
#include "entity.h"
#include "script.h"
#include "shop.h"
#include "stat.h"
#include "utility.h"
#include "zone.h"


/* <MrAndrew> it's 40 cycles, which is about 25 seconds */
#define MAX_IDLE_TICKS		(40)

struct server s = {0};

static void on_signal_do_nothing(int);
static int load_config_jag(void);
static void load_map_chunk(struct jag_map *, int, int, int);
static void load_map_tile(struct jag_map *, int, int, int, int, int);
static int load_maps_jag(void);
static void usage(void);

static void
usage(void)
{
	fputs("rscsundae [-b basedir] [-d] [-f conffile]\n", stderr);
}

int
main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	char ch;
	char *basedir = "./data";
	char *conffile = "./data/settings.ini";

	while ((ch = getopt(argc, argv, "b:df:")) != -1) {
		switch (ch) {
		case 'b':
			basedir = strdup(optarg);
			assert(basedir != NULL);
			break;
		case 'd':
			daemon(1, 0);
			break;
		case 'f':
			conffile = strdup(optarg);
			assert(conffile != NULL);
			break;
		case '?':
		default:
			usage();
			exit(EXIT_FAILURE);
			break;
		}
	}

	printf("base directory is %s\n", basedir);
	printf("configuration file is %s\n", conffile);

	argc -= optind;
	argv += optind;

	s.name = "RSC Sundae";
	s.bind_addr = "127.0.0.1";
	s.port = 43594;
	s.next_restore = 0;
	s.next_rapid_restore = 0;
	s.next_prayer_drain = 0;

	(void)signal(SIGPIPE, on_signal_do_nothing);

	/* init random number generator */
	raninit(&s.ran, time(NULL));

	init_opcodes_203();
	stat_calculate_table();

#ifndef FUZZ
	if (ini_parse(conffile, server_parse_settings, &s) < 0) {
		fprintf(stderr, "error parsing ./data/settings.ini\n");
		return EXIT_FAILURE;
	}

	(void)chdir(basedir);

	if (database_init(&s.database)) {
		fprintf(stderr, "failed to initialize database\n");
		return EXIT_FAILURE;
	}

	if (load_config_jag() == -1) {
		fprintf(stderr, "error reading config.jag\n");
		fprintf(stderr, "did you run the fetch-jag-files script from the data directory?\n");
		return EXIT_FAILURE;
	}

	if (load_maps_jag() == -1) {
		fprintf(stderr, "error reading maps.jag\n");
		fprintf(stderr, "did you run the fetch-jag-files script from the data directory?\n");
		return EXIT_FAILURE;
	}

	s.lua = script_init(&s);
	if (s.lua == NULL) {
		fprintf(stderr, "failed to start script engine\n");
		return EXIT_FAILURE;
	}

	printf("calculating password hashing params... ");
	if (pwhash_init(&s.hash) != -1) {
		printf("time = %u, memory = %u\n",
		    s.hash.t_cost, s.hash.m_cost);
	} else {
		printf("failed\n");
	}

#else
	s.rsa_exponent = "571fb062048b61721ebfcf1e877153241b70c3aa26edb0f9f06a1b2be07c4e45eaba4fc356ea806cbed298d38613590a53fde0383c3a411758516293240925e5";
	s.rsa_modulus = "88c38748a58228f7261cdc340b5691d7d0975dee0ecdb717609e6bf971eb3fe723ef9d130e4686813739768ad9472eb46d8bfcc042c1a5fcb05e931f632eea5d";

	struct item_config item = {0};

	item.names[0] = "Coins";
	item.name_count = 1;

	s.item_config = &item;
	s.item_config_count = 1;
#endif

	if (rsa_init(&s.rsa, s.rsa_exponent, s.rsa_modulus) == -1) {
		fprintf(stderr, "failed to initialize with rsa private key\n");
		return EXIT_FAILURE;
	}

	printf("starting server on address %s port %d\n",
	    s.bind_addr, s.port);

#ifdef PROFILE
	ProfilerStart("./sundae.prof");
#endif

#ifndef FUZZ
	if (loop_start(&s) == -1) {
		return EXIT_FAILURE;
	}
#else
	struct player *p;
	int fd, len;

	p = player_create(&s, -1);
	if (p == NULL) {
		return EXIT_FAILURE;
	}
	fd = open(argv[0], O_RDONLY);
	if (fd == -1) {
		return EXIT_FAILURE;
	}
	len = read(fd, p->inbuf, 5000);
	if (len < 0) {
		return EXIT_FAILURE;
	}
	printf("read %zu bytes\n", len);
	p->inbuf_processed = 0;
	p->inbuf_len = (size_t)len;
	player_parse_incoming(p);
#endif
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
		if (s.players[i] == NULL) {
			continue;
		}
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
server_send_pm(struct player *from, int64_t target, const char *msg)
{
	struct player *targetp;

	targetp = server_find_player_name37(target);
	if (targetp == NULL) {
		return;
	}
	if (player_is_blocked(targetp, from->name, targetp->block_private)) {
		return;
	}
	player_send_pm(targetp, from->name, msg);
}

void
server_tick(void)
{
	bool drain_tick;
	bool restore_tick;
	bool rapid_restore_tick;
	uint64_t start_time;
	uint64_t time_delay = 0;

	start_time = get_time_ms();

	s.tick_counter++;

	drain_tick = s.tick_counter >= s.next_prayer_drain;
	restore_tick = s.tick_counter >= s.next_restore;
	rapid_restore_tick = s.tick_counter >= s.next_rapid_restore;

	if (s.last_tick != 0) {
		int64_t difference = start_time - s.last_tick;
		if (difference > TICK_RATE) {
			time_delay = difference - TICK_RATE;
		}
	}

	/* remove stale temporary ground items */
	for (size_t i = 0; i < s.temp_item_count; ++i) {
		if (s.tick_counter < (s.temp_items[i].creation_time + 200)) {
			continue;
		}
		s.temp_item_count--;
		for (size_t j = i; j < s.temp_item_count; ++j) {
			s.temp_items[j] = s.temp_items[j + 1];
		}
	}

	/* restock shops */
	for (size_t i = 0; i < s.shop_config_count; ++i) {
		shop_process(&s.shop_config[i]);
	}

	script_tick(s.lua);

	for (int i = 0; i < s.max_npc_id; ++i) {
		if (s.npcs[i] == NULL) {
			continue;
		}
		if (s.npcs[i]->respawn_time == 0) {
			npc_process_combat(s.npcs[i]);
			npc_process_movement(s.npcs[i]);
		}
	}

	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		if (s.tick_counter > (s.players[i]->last_packet + MAX_IDLE_TICKS) ||
		    (s.movement_timer &&
			s.players[i]->mob.movement_timer > 600)) {
			/*
			 * in this replay:
			 * Zephyr (redacted chat) replays/1m d4 r4ng3r/06-14-2018 10.47.53 testing 5 minute logout bug
			 * he gets kicked roughly 100 ticks after the warning
			 */
			player_attempt_logout(s.players[i]);
		}
		if (s.players[i]->logout_confirmed) {
			char name[32];

			server_register_logout(s.players[i]->name);
			mod37_namedec(s.players[i]->name, name);
			printf("removed player %s\n", name);
			player_destroy(s.players[i]);
			s.players[i] = NULL;
			s.player_count--;
			continue;
		}
		net_player_recv(s.players[i]);
		player_parse_incoming(s.players[i]);
		player_process_combat(s.players[i]);
		player_process_action(s.players[i]);
		player_process_movement(s.players[i]);
		script_process(s.lua, s.players[i]);
		if (drain_tick) {
			player_prayer_drain(s.players[i]);
		}
		if (restore_tick) {
			player_slow_restore(s.players[i]);
		}
		if (rapid_restore_tick) {
			player_rapid_restore(s.players[i]);
		}
		if (s.players[i]->skull_timer > 0) {
			s.players[i]->skull_timer--;
			if (s.players[i]->skull_timer == 0) {
				s.players[i]->skulled = false;
				s.players[i]->appearance_changed = true;
			}
		}
		if (s.movement_timer &&
		    (s.players[i]->mob.movement_timer++) == 500) {
			player_send_message(s.players[i],
			    "@cya@You have been standing here for 5 mins! "
			    "Please move to a new area");
		}
	}

	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL ||
		    s.players[i]->login_stage != LOGIN_STAGE_GOT_LOGIN) {
			continue;
		}
		if (s.players[i]->plane_changed) {
			player_send_plane_init(s.players[i]);
		}
		if (s.players[i]->inv_changed) {
			player_send_inv(s.players[i]);
		}
		if (s.players[i]->stats_changed) {
			player_send_init_stats(s.players[i]);
		}
		if (s.players[i]->bonus_changed) {
			player_send_equip_bonuses(s.players[i]);
		}
		if (s.players[i]->prayers_changed) {
			player_send_prayers(s.players[i]);
		}
		if (s.players[i]->partner_offer_changed) {
			player_send_partner_trade_offer(s.players[i]);
		}
		if (s.players[i]->shop != NULL && s.players[i]->shop->changed) {
			player_send_shop(s.players[i],
			    s.players[i]->shop->name);
		}
		player_send_movement(s.players[i]);
		player_send_appearance_update(s.players[i]);
		player_send_npc_movement(s.players[i]);
		player_send_npc_appearance_update(s.players[i]);
		player_send_locs(s.players[i]);
		player_send_ground_items(s.players[i]);
		player_send_bounds(s.players[i]);
		net_player_send(s.players[i]);
	}

	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] == NULL) {
			continue;
		}
		if (s.players[i]->spell_timer > 0) {
			s.players[i]->spell_timer--;
		}
		s.players[i]->chat_type = CHAT_TYPE_NONE;
		s.players[i]->mob.chat_len = 0;
		s.players[i]->appearance_changed = false;
		s.players[i]->plane_changed = false;
		s.players[i]->stats_changed = false;
		s.players[i]->inv_changed = false;
		s.players[i]->bonus_changed = false;
		s.players[i]->prayers_changed = false;
		s.players[i]->partner_offer_changed = false;
		s.players[i]->teleported = false;
		s.players[i]->projectile_target_npc = UINT16_MAX;
		s.players[i]->projectile_target_player = UINT16_MAX;
		s.players[i]->bubble_id = UINT16_MAX;
		s.players[i]->mob.moved = false;
		s.players[i]->mob.damage = UINT8_MAX;
		s.players[i]->mob.prev_dir = s.players[i]->mob.dir;
		s.players[i]->last_update = s.tick_counter;
		if (s.players[i]->login_date != 0) {
			s.players[i]->save_timer++;
			if (s.players[i]->save_timer == 500) {
				(void)database_save_player(&s.database, s.players[i]);
				s.players[i]->save_timer = 0;
			}
		}
	}

	for (int i = 0; i < s.max_npc_id; ++i) {
		if (s.npcs[i] == NULL) {
			continue;
		}
		if (s.npcs[i]->regen_timer > 0) {
			s.npcs[i]->regen_timer--;
		} else {
			for (int j = 0; j < MAX_SKILL_ID; ++j) {
				if (s.npcs[i]->mob.cur_stats[j] <
				    s.npcs[i]->mob.base_stats[j]) {
					s.npcs[i]->mob.cur_stats[j]++;
				}
			}
			s.npcs[i]->regen_timer =
			    s.npcs[i]->config->regeneration;
		}
		if (s.npcs[i]->respawn_time > 0) {
			s.npcs[i]->respawn_time--;
		}
		s.npcs[i]->mob.chat_len = 0;
		s.npcs[i]->mob.damage = UINT8_MAX;
		s.npcs[i]->mob.moved = false;
		s.npcs[i]->mob.prev_dir = s.npcs[i]->mob.dir;
	}

	for (size_t i = 0; i < s.shop_config_count; ++i) {
		s.shop_config[i].changed = false;
	}

	if (drain_tick) {
		s.next_prayer_drain = s.tick_counter + 3;
	}

	if (restore_tick) {
		s.next_restore = s.tick_counter + 100;
	}

	if (rapid_restore_tick) {
		s.next_rapid_restore = s.tick_counter + 50;
	}

	s.last_tick = get_time_ms();
	if ((s.last_tick - start_time) > 50) {
		printf("warning: server running slowly,");
		printf(" processing took %llu milliseconds\n",
		    (unsigned long long)(s.last_tick - start_time));
	}

#ifdef PROFILE
	ProfilerFlush();
#endif

	time_delay += s.last_tick - start_time;

	if (time_delay > TICK_RATE) {
		loop_set_delay(0);
	} else {
		loop_set_delay(TICK_RATE - time_delay);
	}
}

static int
load_config_jag(void)
{
	struct jag_archive archive = {0};
	struct jag_entry entry = {0};
	FILE *f = NULL;

	f = fopen("./config46.jag", "rb");
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

	if (jag_find_entry(&archive, "location.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.loc_config = config_parse_locs((char *)entry.data,
	    entry.unpacked_len, &s.loc_config_count);
	if (s.loc_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu locs\n",
	    s.loc_config_count);

	if (jag_find_entry(&archive, "boundary.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.bound_config = config_parse_bounds((char *)entry.data,
	    entry.unpacked_len, &s.bound_config_count);
	if (s.bound_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu bounds\n",
	    s.bound_config_count);

	if (jag_find_entry(&archive, "projectile.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.projectile_config = config_parse_projectiles((char *)entry.data,
	    entry.unpacked_len, &s.projectile_config_count);
	if (s.projectile_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu projectiles\n",
	    s.projectile_config_count);

	if (jag_find_entry(&archive, "npc.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.npc_config = config_parse_npcs((char *)entry.data,
	    entry.unpacked_len, &s.npc_config_count,
	    s.entity_config, s.entity_config_count,
	    s.item_config, s.item_config_count);
	if (s.npc_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu npcs\n",
	    s.npc_config_count);

	if (jag_find_entry(&archive, "spells.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.spell_config = config_parse_spells((char *)entry.data,
	    entry.unpacked_len, &s.spell_config_count,
	    s.item_config, s.item_config_count);
	if (s.spell_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu spells\n",
	    s.spell_config_count);

	if (jag_find_entry(&archive, "shop.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.shop_config = config_parse_shops((char *)entry.data,
	    entry.unpacked_len, &s.shop_config_count,
	    s.item_config, s.item_config_count);
	if (s.shop_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu shops\n",
	    s.shop_config_count);

	if (jag_find_entry(&archive, "floor.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}
	s.floor_config = config_parse_floors((char *)entry.data,
	    entry.unpacked_len, &s.floor_config_count);
	if (s.floor_config == NULL) {
		goto err;
	}
	printf("read configuration for %zu floors\n",
	    s.floor_config_count);

	if (jag_find_entry(&archive, "words.txt", &entry) == -1 ||
	    jag_unpack_entry(&entry) == -1) {
		goto err;
	}

	bool new_word = true;
	for (size_t i = 0; i < entry.unpacked_len; ++i) {
		char *ch = entry.data;
		if (ch[i] == '\n' || ch[i] == '\r') {
			ch[i] = '\0';
			new_word = true;
			continue;
		}
		if (new_word) {
			s.words[s.num_words++] = ch + i;
		}
		new_word = false;
	}
	printf("read %zu good words\n", s.num_words);

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

static void
load_map_tile(struct jag_map *chunk,
    int tile_x, int tile_y, int global_x, int global_y, int plane)
{
	struct loc loc;
	struct loc_config *loc_config;
	struct bound bound;
	struct bound_config *bound_config;
	struct floor_config *floor_config;
	struct ground_item item;
	uint32_t object_type;
	uint16_t object_dir;
	int world_x, world_y;
	int ind;

	world_x = global_x;
	world_y = global_y - (plane * PLANE_LEVEL_INC);

	ind = tile_x * JAG_MAP_CHUNK_SIZE + tile_y;

	if (chunk->tiles[ind].overlay) {
		object_type = chunk->tiles[ind].overlay - 1;
		floor_config = server_floor_config_by_id(object_type);
		assert(floor_config != NULL);
		if (floor_config->blocked) {
			s.adjacency[plane][world_x][world_y] |= ADJ_BLOCK;
		}
	}

	if (chunk->tiles[ind].roof) {
		s.roofs[world_x][world_y] = chunk->tiles[ind].roof;
	}

	object_type = chunk->tiles[ind].bound_diag;
	object_dir = chunk->tiles[ind].loc_direction;
	if (chunk->tiles[ind].bound_vert) {
		bound.x = global_x;
		bound.y = global_y;
		bound.id =  chunk->tiles[ind].bound_vert - 1;
		bound.dir = BOUND_DIR_VERT;
		bound_config = server_bound_config_by_id(bound.id);
		if (bound_config != NULL) {
			if (bound_config->interactive) {
				server_add_bound(&s, &bound);
			} else {
				if (bound_config->block) {
					s.adjacency[plane][world_x][world_y - 1]
					    |= ADJ_BLOCK_VERT;
				}
				if (bound_config->block_projectile) {
					s.adjacency[plane][world_x][world_y - 1]
					    |= ADJ_BLOCK_SIGHT_VERT;
				}
			}
		}
	}
	if (chunk->tiles[ind].bound_horiz) {
		bound.x = global_x;
		bound.y = global_y;
		bound.id =  chunk->tiles[ind].bound_horiz - 1;
		bound.dir = BOUND_DIR_HORIZ;
		bound_config = server_bound_config_by_id(bound.id);
		if (bound_config != NULL) {
			if (bound_config->interactive) {
				server_add_bound(&s, &bound);
			} else {
				if (bound_config->block) {
					s.adjacency[plane][world_x - 1][world_y]
					    |= ADJ_BLOCK_HORIZ;
				}
				if (bound_config->block_projectile) {
					s.adjacency[plane][world_x - 1][world_y]
					    |= ADJ_BLOCK_SIGHT_HORIZ;
				}
			}
		}
	}
	if (object_type > JAG_MAP_DIAG_LOC) {
		int width, height;
		int max_x, max_y;

		loc.id = object_type - JAG_MAP_DIAG_LOC - 1;
		loc.orig_id = loc.id;
		loc.x = global_x;
		loc.y = global_y;
		loc.dir = object_dir;

		/*
		 * this is a hack to normalize mining differences between
		 * client versions, and also not have some rocks as
		 * "always mined
		 */
		switch (loc.id) {
		case LOC_TINROCK3:
			loc.id = loc.orig_id = LOC_TINROCK1;
			break;
		case LOC_COPPERROCK3:
			loc.id = loc.orig_id = LOC_COPPERROCK1;
			break;
		case LOC_IRONROCK3:
			loc.id = loc.orig_id = LOC_IRONROCK1;
			break;
		case LOC_GOLDROCK3:
			loc.id = loc.orig_id = LOC_GOLDROCK1;
			break;
		case LOC_CLAYROCK3:
			loc.id = loc.orig_id = LOC_CLAYROCK1;
			break;
		case LOC_SILVERROCK3:
			loc.id = loc.orig_id = LOC_SILVERROCK1;
			break;
		case LOC_COALROCK3:
			loc.id = loc.orig_id = LOC_COALROCK1;
			break;
		case LOC_MITHRILROCK3:
			loc.id = loc.orig_id = LOC_MITHRILROCK1;
			break;
		case LOC_ADAMANTITEROCK3:
			loc.id = loc.orig_id = LOC_ADAMANTITEROCK1;
			break;
		case LOC_RUNITEROCK3:
			loc.id = loc.orig_id = LOC_RUNITEROCK1;
			break;
		}

		loc_config = server_loc_config_by_id(loc.id);
		if (loc_config == NULL) {
			return;
		}
		server_add_loc(&s, &loc);
		if (object_dir == MOB_DIR_NORTH ||
		    object_dir == MOB_DIR_SOUTH) {
			width = loc_config->width;
			height = loc_config->height;
		} else {
			width = loc_config->height;
			height = loc_config->width;
		}
		max_x = tile_x + width;
		if (max_x >= JAG_MAP_CHUNK_SIZE) {
			max_x = JAG_MAP_CHUNK_SIZE;
		}
		max_y = tile_y + height;
		if (max_y >= JAG_MAP_CHUNK_SIZE) {
			max_y = JAG_MAP_CHUNK_SIZE;
		}
		for (int x = tile_x; x < max_x; ++x) {
			for (int y = tile_y; y < max_y; ++y) {
				int ind2;
				uint32_t n;

				if (x == tile_x && y == tile_y) {
					continue;
				}

				/*
				 * have to clear up adjacent tiles - apparently
				 * the original map editor puts locs on
				 * every tile their width and height occupy,
				 * and we don't want duplicates
				 */
				ind2 = x * JAG_MAP_CHUNK_SIZE + y;
				n = chunk->tiles[ind2].bound_diag;
				if (n == object_type) {
					chunk->tiles[ind2].bound_diag = 0;
				}
			}
		}
	} else if (object_type > JAG_MAP_DIAG_ITEM) {
		struct loc *l;

		l = server_find_loc(global_x, global_y - 1);
		item.id = object_type - JAG_MAP_DIAG_ITEM - 1;
		item.unique_id = s.ground_item_counter++;
		item.x = global_x;
		item.y = global_y;
		item.on_surface = false;
		if (l != NULL) {
			loc_config = server_loc_config_by_id(l->id);
			assert(loc_config != NULL);
			if (loc_config->surface_height > 0) {
				item.y = global_y - 1;
				item.on_surface = true;
			}
		}
		item.respawn = true;
		item.stack = 1;
		item.owner = UINT16_MAX;
		item.creation_time = 0;
		item.respawn_time = 0;
		server_add_item_respawn(&item);
	} else if (object_type > JAG_MAP_DIAG_NPC) {
		server_add_npc(object_type - JAG_MAP_DIAG_NPC - 1,
		    global_x, global_y);
	} else if (object_type > JAG_MAP_DIAG_INVERSE) {
		bound.x = global_x;
		bound.y = global_y;
		bound.id =  chunk->tiles[ind].bound_diag -
		    1 - JAG_MAP_DIAG_INVERSE;
		bound.dir = BOUND_DIR_DIAG_NW_SE;
		bound_config = server_bound_config_by_id(bound.id);
		if (bound_config != NULL) {
			if (bound_config->interactive) {
				server_add_bound(&s, &bound);
			} else {
				if (bound_config->block_projectile) {
					s.adjacency[plane][world_x][world_y] |=
					    ADJ_BLOCK_SIGHT;
				}
				if (bound_config->block) {
					s.adjacency[plane][world_x][world_y] |=
					    ADJ_BLOCK;
				}
			}
		}
	} else {
		bound.x = global_x;
		bound.y = global_y;
		bound.id =  chunk->tiles[ind].bound_diag - 1;
		bound.dir = BOUND_DIR_DIAG_NE_SW;
		bound_config = server_bound_config_by_id(bound.id);
		if (bound_config != NULL) {
			if (bound_config->interactive) {
				server_add_bound(&s, &bound);
			} else {
				if (bound_config->block_projectile) {
					s.adjacency[plane][world_x][world_y] |=
					    ADJ_BLOCK_SIGHT;
				}
				if (bound_config->block) {
					s.adjacency[plane][world_x][world_y] |=
					    ADJ_BLOCK;
				}
			}
		}
	}
}

static void
load_map_chunk(struct jag_map *chunk, int chunk_x, int chunk_y, int plane)
{
	int global_x, global_y;

	for (int x = 0; x < JAG_MAP_CHUNK_SIZE; ++x) {
		for (int y = 0; y < JAG_MAP_CHUNK_SIZE; ++y) {
			global_x = ((chunk_x - JAG_MAP_CHUNK_SIZE) *
			    JAG_MAP_CHUNK_SIZE) + x;
			global_y = (plane * PLANE_LEVEL_INC) +
			    ((chunk_y - 37) * JAG_MAP_CHUNK_SIZE) + y;
			load_map_tile(chunk, x, y, global_x, global_y, plane);
		}
	}
}

static int
load_maps_jag(void)
{
	struct jag_archive archive = {0};
	struct jag_entry entry = {0};
	FILE *f = NULL;

	f = fopen("./maps27.jag", "rb");
	if (f == NULL) {
		goto err;
	}
	if (jag_unpack_file(f, &archive) == -1) {
		goto err;
	}
	fclose(f);
	f = NULL;
	for (int plane = 0; plane < 4; ++plane) {
		for (int x = 49; x < 64; ++x) {
			for (int y = 40; y < 64; ++y) {
				struct jag_map chunk;
				char file[64];

				(void)snprintf(file, sizeof(file),
				    "m%d%d%d%d%d.jm", plane,
				    x / 10, x % 10, y / 10, y % 10);
				if (jag_find_entry(&archive,
					file, &entry) == -1 ||
				    jag_unpack_entry(&entry) == -1) {
					continue;
				}

				if (jag_map_read_jm(&chunk,
				    entry.data, entry.unpacked_len) != -1) {
					load_map_chunk(&chunk, x, y, plane);
				}

				if (entry.must_free) {
					free(entry.data);
					entry.data = NULL;
				}
			}
		}
	}
	if (archive.must_free) {
		free(archive.data);
		archive.data = NULL;
	}
	return 0;
err:
	if (archive.must_free) {
		free(archive.data);
		archive.data = NULL;
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

struct item_config *
server_find_item_config(const char *name)
{
	int id = config_find_item(name, s.item_config, s.item_config_count);
	if (id == -1) {
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

struct loc_config *
server_loc_config_by_id(int id)
{
	if (id < 0 || id >= (int)s.loc_config_count) {
		return NULL;
	}
	return &s.loc_config[id];
}

struct floor_config *
server_floor_config_by_id(int id)
{
	if (id < 0 || id >= (int)s.floor_config_count) {
		return NULL;
	}
	return &s.floor_config[id];
}

struct bound_config *
server_bound_config_by_id(int id)
{
	if (id < 0 || id >= (int)s.bound_config_count) {
		return NULL;
	}
	return &s.bound_config[id];
}

struct bound_config *
server_find_bound_config(const char *name)
{
	for (size_t i = 0; i < s.bound_config_count; ++i) {
		for (size_t j = 0; j < s.bound_config[i].name_count; ++j) {
			if (strcasecmp(name, s.bound_config[i].names[j]) == 0) {
				return &s.bound_config[i];
			}
		}
	}
	return NULL;
}

struct loc_config *
server_find_loc_config(const char *name)
{
	for (size_t i = 0; i < s.loc_config_count; ++i) {
		for (size_t j = 0; j < s.loc_config[i].name_count; ++j) {
			if (strcasecmp(name, s.loc_config[i].names[j]) == 0) {
				return &s.loc_config[i];
			}
		}
	}
	return NULL;
}

struct spell_config *
server_spell_config_by_id(int id)
{
	if (id < 0 || id >= (int)s.spell_config_count) {
		return NULL;
	}
	return &s.spell_config[id];
}

struct npc_config *
server_npc_config_by_id(int id)
{
	if (id < 0 || id >= (int)s.npc_config_count) {
		return NULL;
	}
	return &s.npc_config[id];
}

struct npc_config *
server_find_npc_config(const char *name)
{
	for (size_t i = 0; i < s.npc_config_count; i++) {
		for (size_t j = 0; j < s.npc_config[i].name_count; j++) {
			if (strcasecmp(s.npc_config[i].names[j], name) == 0) {
				return &s.npc_config[i];
			}
		}
	}
	return NULL;
}

struct projectile_config *
server_find_projectile(const char *name)
{
	if (name[0] == '_') {
		return NULL;
	}
	for (size_t i = 0; i < s.projectile_config_count; ++i) {
		if (strcasecmp(s.projectile_config[i].name, name) == 0) {
			return &s.projectile_config[i];
		}
	}
	return NULL;
}

struct shop_config *
server_find_shop(const char *name)
{
	for (size_t i = 0; i < s.shop_config_count; ++i) {
		if (strcasecmp(s.shop_config[i].name, name) == 0) {
			return &s.shop_config[i];
		}
	}
	return NULL;
}

int
server_add_npc(int id, int x, int y)
{
	struct npc *npc;
	struct zone *zone;

	for (size_t i = 0; i < MAXNPCS; ++i) {
		if (s.npcs[i] != NULL) {
			continue;
		}
		npc = calloc(1, sizeof(struct npc));
		if (npc == NULL) {
			return -1;
		}
		npc->config = server_npc_config_by_id(id);
		assert(npc->config != NULL);
		npc->spawn_x = x;
		npc->spawn_y = y;
		npc->mob.id = (uint16_t)i;
		npc->mob.x = x;
		npc->mob.y = y;
		npc->mob.server = &s;
		npc->mob.is_npc = true;
		npc->mob.damage = UINT8_MAX;
		npc->mob.following_player = -1;
		npc->mob.following_npc = -1;
		npc->mob.cur_stats[SKILL_ATTACK] =
		    npc->mob.base_stats[SKILL_ATTACK] = npc->config->attack;
		npc->mob.cur_stats[SKILL_DEFENSE] =
		    npc->mob.base_stats[SKILL_DEFENSE] = npc->config->defense;
		npc->mob.cur_stats[SKILL_STRENGTH] =
		    npc->mob.base_stats[SKILL_STRENGTH] = npc->config->strength;
		npc->mob.cur_stats[SKILL_HITS] =
		    npc->mob.base_stats[SKILL_HITS] = npc->config->hits;
		s.npcs[i] = npc;
		zone = server_find_zone(x, y);
		if (zone != NULL) {
			zone_add_npc(zone, i);
		}
		if (i > s.max_npc_id) {
			s.max_npc_id = i + 1;
		}
		return i;
	}
	printf("WARNING: failed to add NPC, too many\n");
	return -1;
}

int
server_add_temp_item(struct player *owner, int x, int y, int id, uint32_t stack)
{
	struct ground_item item = {0};
	int plane = 0;
	int w_y = y;

	item.id = id;
	item.x = x;
	item.y = y;
	item.stack = stack;
	item.owner = owner != NULL ? owner->mob.id : UINT16_MAX;
	item.creation_time = s.tick_counter;
	item.unique_id = s.ground_item_counter++;

	while (w_y > PLANE_LEVEL_INC) {
		w_y -= PLANE_LEVEL_INC;
		plane++;
	}

	if ((s.adjacency[plane][x][w_y] & ADJ_BLOCK) != 0) {
		item.on_surface = true;
	}

	if (s.temp_item_count >= s.temp_item_max) {
		size_t n = s.temp_item_max * 2;
		if (n == 0) {
			n = 128;
		}
		if (reallocarr(&s.temp_items,
		    n, sizeof(struct ground_item)) == -1) {
			return -1;
		}
		s.temp_item_max = n;
	}

	s.temp_items[s.temp_item_count++] = item;
	return 0;
}

void
server_remove_temp_item(uint64_t unique_id)
{
	for (size_t i = 0; i < s.temp_item_count; ++i) {
		if (s.temp_items[i].unique_id != unique_id) {
			continue;
		}
		s.temp_item_count--;
		for (size_t j = i; j < s.temp_item_count; ++j) {
			s.temp_items[j] = s.temp_items[j + 1];
		}
	}
}


double
server_random(void)
{
	/* emulation of java's Math.random() */
	return ranval(&s.ran) / (double)UINT32_MAX;
}

void
server_sysmes(const char *mes)
{
	for (int i = 0; i < s.max_player_id; ++i) {
		if (s.players[i] != NULL) {
			player_send_mesbox(s.players[i], mes);
		}
	}
}

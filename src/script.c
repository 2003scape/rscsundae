#include <assert.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <string.h>
#include "ext/flea.h"
#include "script.h"
#include "server.h"
#include "inventory.h"
#include "stat.h"
#include "utility.h"
#include "zone.h"

static struct server *serv;
static int script_say(lua_State *);
static int script_npcattack(lua_State *);
static int script_npcbusy(lua_State *);
static int script_npcunbusy(lua_State *);
static int script_playerbusy(lua_State *);
static int script_playerunbusy(lua_State *);
static int script_npcsay(lua_State *);
static int script_male(lua_State *);
static int script_nearnpc(lua_State *);
static int script_random(lua_State *);
static int script_give(lua_State *);
static int script_remove(lua_State *);
static int script_sellinv(lua_State *);
static int script_held(lua_State *);
static int script_worn(lua_State *);
static int script_advancestat(lua_State *);
static int script_healstat(lua_State *);
static int script_addstat(lua_State *);
static int script_substat(lua_State *);
static int script_npchealstat(lua_State *);
static int script_npcaddstat(lua_State *);
static int script_npcsubstat(lua_State *);
static int script_statatleast(lua_State *);
static int script_statup(lua_State *);
static int script_statdown(lua_State *);
static int script_statrandom(lua_State *);
static int script_npcstatup(lua_State *);
static int script_npcstatdown(lua_State *);
static int script_thinkbubble(lua_State *);
static int script_boundaryteleport(lua_State *);
static int script_upstairs(lua_State *);
static int script_downstairs(lua_State *);
static int script_changelevelup(lua_State *);
static int script_changeleveldown(lua_State *);
static int script_openshop(lua_State *);
static int script_giveqp(lua_State *);
static int script_changebound(lua_State *);
static int script_changeloc(lua_State *);
static int script_restoreloc(lua_State *);
static int script_shootplayer(lua_State *);
static int script_shootnpc(lua_State *);
static int script_multi(lua_State *);
static int script_addobject(lua_State *);
static int script_delobject(lua_State *);
static int script_takeobject(lua_State *);
static int script_getvar(lua_State *);
static int script_setvar(lua_State *);
static int script_addnpc(lua_State *);
static int script_playercoord(lua_State *);
static int script_teleport(lua_State *);
static int script_qp(lua_State *);
static struct player *id_to_player(lua_Integer);
static struct npc *id_to_npc(lua_Integer);
static void safe_call(lua_State *, int, int, int);
static int script_panic(lua_State *);
static lua_Integer script_checkinteger(lua_State *, int);
static const char *script_checkstring(lua_State *, int);

static lua_Integer
script_checkinteger(lua_State *L, int index)
{
	/* replacement for luaL_checkinteger with error handling */
	int isnum = false;
	lua_Integer result;

	isnum = false;
	result = lua_tointegerx(L, index, &isnum);
	if (!isnum) {
		printf("Error in Lua script: argument %d is not an integer!\n", index);
		return 0;
	}
	return result;
}

static const char *
script_checkstring(lua_State *L, int index)
{
	/* replacement for luaL_checkstring with error handling */
	const char *result;

	if (!lua_isstring(L, index)) {
		printf("Error in Lua script: argument %d is not a string!\n", index);
		return "";
	}
	result = lua_tostring(L, index);
	return result;
}

static int
script_panic(lua_State *L)
{
	printf("Error in Lua script: %s\n", lua_tostring(L, -1));
	fflush(stdout);
	return 0;
}

static void
safe_call(lua_State *L, int nargs, int nresults, int player_id)
{
	int ret = lua_pcall(L, nargs, nresults, 0);
	if (ret != 0) {
		printf("Error in Lua script: %s\n", lua_tostring(L, -1));
		script_cancel(L, player_id);
	}
}

static struct player *
id_to_player(lua_Integer id)
{
	if (id < 0 || id >= MAXPLAYERS) {
		printf("script warning: player id %lld out of range\n", id);
		return NULL;
	}
	return serv->players[id];
}

static struct npc *
id_to_npc(lua_Integer id)
{
	if (id < 0 || id >= MAXNPCS) {
		printf("script warning: npc id %lld out of range\n", id);
		return NULL;
	}
	return serv->npcs[id];
}

static int
script_blocked(lua_State *L)
{
	lua_Integer x, y;
	int plane = 0;

	x = script_checkinteger(L, 1);
	y = script_checkinteger(L, 2);

	if (server_find_loc(x, y) != NULL) {
		lua_pushboolean(L, true);
		return 1;
	}

	while (y > PLANE_LEVEL_INC) {
		y -= PLANE_LEVEL_INC;
		plane++;
	}

	if (serv->adjacency[plane][x][y] != 0) {
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushboolean(L, false);
	return 1;
}

static int
script_addloc(lua_State *L)
{
	const char *name;
	struct loc_config *config;
	struct loc loc = {0};

	name = script_checkstring(L, 1);
	loc.x = script_checkinteger(L, 2);
	loc.y = script_checkinteger(L, 3);

	config = server_find_loc_config(name);
	if (config == NULL) {
		printf("script warning: loc %s is undefined\n", name);
		return 0;
	}

	loc.id = config->id;
	loc.orig_id = loc.id;

	if (server_find_loc(loc.x, loc.y) != NULL) {
		printf("script warning: tried to spawn loc at %d %d but it's blocked\n",
			loc.x, loc.y);
		return 0;
	}

	server_add_loc(serv, &loc);
	return 0;
}

static int
script_delloc(lua_State *L)
{
	lua_Integer x, y;

	x = script_checkinteger(L, 1);
	y = script_checkinteger(L, 2);

	server_del_loc(x, y);
	return 0;
}

static int
script_addobject(lua_State *L)
{
	lua_Integer id, amount, x, y;
	const char *name;
	struct item_config *config;
	struct player *p;

	id = script_checkinteger(L, 1);
	name = script_checkstring(L, 2);
	amount = script_checkinteger(L, 3);
	x = script_checkinteger(L, 4);
	y = script_checkinteger(L, 5);

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	config = server_find_item_config(name);
	if (config == NULL) {
		printf("script warning: item %s is undefined\n", name);
		script_cancel(L, id);
		return 0;
	}

	server_add_temp_item(p, x, y, config->id, amount);
	return 0;
}

static int
script_delobject(lua_State *L)
{
	lua_Integer id, x, y;
	const char *name;
	struct item_config *config;
	struct ground_item *item;
	struct player *p;

	id = script_checkinteger(L, 1);
	name = script_checkstring(L, 2);
	x = script_checkinteger(L, 3);
	y = script_checkinteger(L, 4);

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	config = server_find_item_config(name);
	if (config == NULL) {
		printf("script warning: item %s is undefined\n", name);
		script_cancel(L, id);
		return 0;
	}

	item = server_find_ground_item(p, x, y, config->id);
	if (item == NULL) {
		printf("script warning: item %s at %lld %lld is undefined\n",
			name, x, y);
		script_cancel(L, id);
		return 0;
	}

	if (item->respawn) {
		item->respawn_time = p->mob.server->tick_counter +
		    (config->respawn_rate / 5);
	} else {
		server_remove_temp_item(item->unique_id);
	}
	return 0;
}

static int
script_takeobject(lua_State *L)
{
	lua_Integer id, x, y;
	const char *name;
	struct item_config *config;
	struct ground_item *item;
	struct player *p;

	id = script_checkinteger(L, 1);
	name = script_checkstring(L, 2);
	x = script_checkinteger(L, 3);
	y = script_checkinteger(L, 4);

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	config = server_find_item_config(name);
	if (config == NULL) {
		printf("script warning: item %s is undefined\n", name);
		script_cancel(L, id);
		return 0;
	}

	item = server_find_ground_item(p, x, y, config->id);
	if (item == NULL) {
		printf("script warning: item %s at %lld %lld is undefined\n",
			name, x, y);
		script_cancel(L, id);
		return 0;
	}

	player_takeobject(p, item);
	return 0;
}

static int
script_say(lua_State *L)
{
	lua_Integer id = script_checkinteger(L, 1);
	const char *mes = script_checkstring(L, 2);
	size_t len;
	struct player *p;

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	if (mes == NULL) {
		printf("script warning: no string specified for say\n");
		script_cancel(L, id);
		return 0;
	}

	len = strlen(mes);
	if (len > MAX_CHAT_LEN) {
		len = MAX_CHAT_LEN;
	}

	encode_chat_legacy(mes, (uint8_t *)p->mob.chat_enc, len);
	p->mob.chat_compressed_len =
	    chat_compress(mes, p->mob.chat_compressed);
	p->mob.chat_len = len;
	p->chat_type = CHAT_TYPE_QUEST;

	return 0;
}

static int
script_multi(lua_State *L)
{
	lua_Integer id = script_checkinteger(L, 1);
	const char *options[MAX_MULTI_SIZE];
	struct player *p;
	uint8_t option_count = 0;

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	for (int i = 1; i < (MAX_MULTI_SIZE + 1); ++i) {
		lua_geti(L, 2, i);
		if (!lua_isstring(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		options[i - 1] = script_checkstring(L, -1);
		if (options[i - 1] == NULL) {
			printf("script warning: multi option is undefined\n");
			return 0;
		}
		lua_pop(L, 1);
		option_count++;
	}

	player_send_show_multi(p, options, option_count);
	return 0;
}

static int
script_npcattack(lua_State *L)
{
	lua_Integer npc_id = script_checkinteger(L, 1);

	(void)npc_id;
	printf("npcattack\n");

	return 0;
}

static int
script_npcbusy(lua_State *L)
{
	lua_Integer id = script_checkinteger(L, 1);
	struct npc *npc;

	npc = id_to_npc(id);
	if (npc == NULL) {
		printf("script warning: npc %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	npc->busy = true;
	return 0;
}

static int
script_npcunbusy(lua_State *L)
{
	lua_Integer id = script_checkinteger(L, 1);
	struct npc *npc;

	npc = id_to_npc(id);
	if (npc == NULL) {
		printf("script warning: npc %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	npc->busy = false;
	return 0;
}

static int
script_playerbusy(lua_State *L)
{
	lua_Integer id = script_checkinteger(L, 1);
	struct player *p;

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	p->script_active = true;
	return 0;
}

static int
script_playerunbusy(lua_State *L)
{
	lua_Integer id = script_checkinteger(L, 1);
	struct player *p;

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	p->script_active = false;
	return 0;
}

static int
script_npcsay(lua_State *L)
{
	lua_Integer id = script_checkinteger(L, 1);
	const char *mes = script_checkstring(L, 2);
	size_t len;
	struct npc *npc;
	struct player *player;

	npc = id_to_npc(id);
	if (npc == NULL) {
		printf("script warning: npc %lld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	if (mes == NULL) {
		printf("script warning: no string specified for npcsay\n");
		script_cancel(L, id);
		return 0;
	}

	len = strlen(mes);
	if (len > MAX_CHAT_LEN) {
		len = MAX_CHAT_LEN;
	}

	player = id_to_player(npc->talk_target);
	if (player != NULL) {
		/*
		 * XXX: maybe too quick at the moment -
		 * seems should be delayed by 1 tick
		 */
		mob_face(&npc->mob, player->mob.x, player->mob.y);
		mob_face(&player->mob, npc->mob.x, npc->mob.y);
	}

	encode_chat_legacy(mes, (uint8_t *)npc->mob.chat_enc, len);
	npc->mob.chat_compressed_len =
	    chat_compress(mes, npc->mob.chat_compressed);
	npc->mob.chat_len = len;

	return 0;
}

static int
script_male(lua_State *L)
{
	lua_Integer player_id;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, p->gender == MOB_GENDER_MALE);
	return 1;
}

static int
script_nearnpc(lua_State *L)
{
	lua_Integer player_id;
	struct player *p;
	const char *name;
	struct npc *npc;

	player_id = script_checkinteger(L, 1);
	name = script_checkstring(L, 2);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		lua_pushnil(L);
		return 1;
	}

	npc = mob_find_nearby_npc(&p->mob, name);
	if (npc != NULL) {
		mob_face(&npc->mob, p->mob.x, p->mob.y);
		mob_face(&p->mob, npc->mob.x, npc->mob.y);
		lua_pushinteger(L, npc->mob.id);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int
script_random(lua_State *L)
{
	lua_Integer probability;
	lua_Integer result;

	probability = script_checkinteger(L, 1);
	result = (int)(255.0 * server_random());
	lua_pushboolean(L, result < probability);
	return 1;
}

static int
script_randomvar(lua_State *L)
{
	lua_Integer probability;
	double result;

	probability = script_checkinteger(L, 1);
	result = probability * server_random();
	lua_pushinteger(L, (lua_Integer)result);
	return 1;
}

static int
script_mes(lua_State *L)
{
	lua_Integer player_id;
	const char *mes;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	mes = script_checkstring(L, 2);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (mes == NULL) {
		printf("script warning: no string specified for mes\n");
		script_cancel(L, player_id);
		return 0;
	}
	player_send_message(p, mes);
	return 0;
}

static int
script_advancestat(lua_State *L)
{
	lua_Integer player_id;
	lua_Integer stat, base, exp;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	base = script_checkinteger(L, 3);
	exp = script_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	stat_advance(p, stat, base, exp);
	return 0;
}

static int
script_healstat(lua_State *L)
{
	lua_Integer player_id;
	lua_Integer stat, constant, percent;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	constant = script_checkinteger(L, 3);
	percent = script_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	stat_heal(&p->mob, stat, constant, percent);
	player_send_stat(p, stat);
	return 0;
}

static int
script_addstat(lua_State *L)
{
	lua_Integer player_id;
	lua_Integer stat, constant, percent;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	constant = script_checkinteger(L, 3);
	percent = script_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	stat_add(&p->mob, stat, constant, percent);
	player_send_stat(p, stat);
	return 0;
}

static int
script_statatleast(lua_State *L)
{
	lua_Integer player_id, stat, low;
	struct player *p;
	int b;

	player_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	low = script_checkinteger(L, 3);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	b = p->mob.cur_stats[stat] >= low;
	lua_pushboolean(L, b);
	return 1;
}

static int
script_statup(lua_State *L)
{
	lua_Integer player_id, stat;
	struct player *p;
	int b;

	player_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	b = stat_up(&p->mob, stat);
	lua_pushboolean(L, b);
	return 1;
}

static int
script_statdown(lua_State *L)
{
	lua_Integer player_id, stat;
	struct player *p;
	int b;

	player_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	b = stat_down(&p->mob, stat);
	lua_pushboolean(L, b);
	return 1;
}

static int
script_statrandom(lua_State *L)
{
	lua_Integer player_id, stat;
	lua_Integer base, top;
	struct player *p;
	int b;

	player_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	base = script_checkinteger(L, 3);
	top = script_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	b = stat_random(&p->mob, stat, base, top);
	lua_pushboolean(L, b);
	return 1;
}

static int
script_npcstatup(lua_State *L)
{
	lua_Integer npc_id, stat;
	struct npc *npc;
	int b;

	npc_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	npc = id_to_npc(npc_id);
	if (npc == NULL) {
		printf("script warning: npc %lld is undefined\n", npc_id);
		lua_pushboolean(L, false);
		return 1;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		lua_pushboolean(L, false);
		return 1;
	}
	b = stat_up(&npc->mob, stat);
	lua_pushboolean(L, b);
	return 1;
}

static int
script_npcstatdown(lua_State *L)
{
	lua_Integer npc_id, stat;
	struct npc *npc;
	int b;

	npc_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	npc = id_to_npc(npc_id);
	if (npc == NULL) {
		printf("script warning: npc %lld is undefined\n", npc_id);
		lua_pushboolean(L, false);
		return 1;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		lua_pushboolean(L, false);
		return 1;
	}
	b = stat_down(&npc->mob, stat);
	lua_pushboolean(L, b);
	return 1;
}

static int
script_substat(lua_State *L)
{
	lua_Integer player_id;
	lua_Integer stat, constant, percent;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	constant = script_checkinteger(L, 3);
	percent = script_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat != SKILL_HITS) {
		stat_remove(&p->mob, stat, constant, percent);
		player_send_stat(p, stat);
	} else {
		int extra;

		extra = (int)((p->mob.base_stats[stat] *
		    (double)percent) / 100.0);
		player_damage(p, NULL, constant + extra);
	}
	return 0;
}

static int
script_npcaddstat(lua_State *L)
{
	lua_Integer npc_id;
	lua_Integer stat, constant, percent;
	struct npc *npc;

	npc_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	constant = script_checkinteger(L, 3);
	percent = script_checkinteger(L, 4);

	npc = id_to_npc(npc_id);
	if (npc == NULL) {
		printf("script warning: npc %lld is undefined\n", npc_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		return 0;
	}
	stat_add(&npc->mob, stat, constant, percent);
	return 0;
}

static int
script_npchealstat(lua_State *L)
{
	lua_Integer npc_id;
	lua_Integer stat, constant, percent;
	struct npc *npc;

	npc_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	constant = script_checkinteger(L, 3);
	percent = script_checkinteger(L, 4);

	npc = id_to_npc(npc_id);
	if (npc == NULL) {
		printf("script warning: npc %lld is undefined\n", npc_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		return 0;
	}
	stat_heal(&npc->mob, stat, constant, percent);
	return 0;
}

static int
script_npcsubstat(lua_State *L)
{
	lua_Integer npc_id;
	lua_Integer stat, constant, percent;
	struct npc *npc;

	npc_id = script_checkinteger(L, 1);
	stat = script_checkinteger(L, 2);
	constant = script_checkinteger(L, 3);
	percent = script_checkinteger(L, 4);

	npc = id_to_npc(npc_id);
	if (npc == NULL) {
		printf("script warning: npc %lld is undefined\n", npc_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %lld\n", stat);
		return 0;
	}
	if (stat != SKILL_HITS) {
		stat_remove(&npc->mob, stat, constant, percent);
	} else {
		int extra;

		extra = (int)((npc->mob.base_stats[stat] *
		    (double)percent) / 100.0);
		npc_damage(npc, NULL, constant + extra);
	}
	return 0;
}


static int
script_getvar(lua_State *L)
{
	lua_Integer player_id;
	const char *varname;
	struct player *p;
	uint32_t value;

	player_id = script_checkinteger(L, 1);
	varname = script_checkstring(L, 2);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	value = player_variable_get(p, varname);
	lua_pushinteger(L, value);
	return 1;
}

static int
script_setvar(lua_State *L)
{
	lua_Integer player_id;
	lua_Integer varvalue;
	const char *varname;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	varname = script_checkstring(L, 2);
	varvalue = script_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	player_variable_set(p, varname, varvalue);
	return 0;
}

static int
script_give(lua_State *L)
{
	lua_Integer amount;
	lua_Integer player_id;
	const char *item_name;
	struct player *p;
	struct item_config *item;

	player_id = script_checkinteger(L, 1);
	item_name = script_checkstring(L, 2);
	amount = script_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	item = server_find_item_config(item_name);
	if (item == NULL) {
		printf("script warning: item %s is undefined\n", item_name);
		script_cancel(L, player_id);
		return 0;
	}

	player_inv_give(p, item, amount);
	return 0;
}

static int
script_remove(lua_State *L)
{
	const char *item_name;
	lua_Integer amount;
	lua_Integer player_id;
	struct player *p;
	struct item_config *item;

	player_id = script_checkinteger(L, 1);
	item_name = script_checkstring(L, 2);
	amount = script_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	item = server_find_item_config(item_name);
	if (item == NULL) {
		printf("script warning: item %s is undefined\n", item_name);
		script_cancel(L, player_id);
		return 0;
	}

	player_inv_remove(p, item, amount);
	return 0;
}

static int
script_sellinv(lua_State *L)
{
	const char *item_name;
	lua_Integer player_id, percentage;
	struct player *p;
	struct item_config *item, *coins;
	double value;

	player_id = script_checkinteger(L, 1);
	item_name = script_checkstring(L, 2);
	percentage = script_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	coins = server_find_item_config("coins");
	assert(coins != NULL);

	item = server_find_item_config(item_name);
	if (item == NULL) {
		printf("script warning: item %s is undefined\n", item_name);
		script_cancel(L, player_id);
		return 0;
	}

	value = item->value * (percentage / 100.0);

	/*
	 * packet order verified, see replay:
	 * Logg/Tylerbeg/06-13-2018 20.09.59 high alch from 55 to 60 and I got a dmed lol
	 */
	if (item->weight > 0) {
		player_inv_remove(p, item, 1);
		player_inv_give(p, coins, value);
	} else {
		for (int i = 0; i < p->inv_count; ++i) {
			if (p->inventory[i].id == item->id) {
				player_inv_remove(p, item,
				    p->inventory[i].stack);
				player_inv_give(p, coins,
				    p->inventory[i].stack * value);
				break;
			}
		}
	}
	return 0;
}

static int
script_displaybalance(lua_State *L)
{
	lua_Integer player_id;
	struct player *p;

	player_id = script_checkinteger(L, 1);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	player_send_show_bank(p);
	return 0;
}

static int
script_held(lua_State *L)
{
	const char *item_name;
	lua_Integer amount;
	lua_Integer player_id;
	struct player *p;
	struct item_config *item;
	int result;

	player_id = script_checkinteger(L, 1);
	item_name = script_checkstring(L, 2);
	amount = script_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	item = server_find_item_config(item_name);
	if (item == NULL) {
		printf("script warning: item %s is undefined\n", item_name);
		script_cancel(L, player_id);
		return 0;
	}

	result = player_inv_held(p, item, amount);
	lua_pushboolean(L, result);
	return 1;
}

static int
script_worn(lua_State *L)
{
	const char *item_name;
	lua_Integer player_id;
	struct player *p;
	struct item_config *item;
	int result;

	player_id = script_checkinteger(L, 1);
	item_name = script_checkstring(L, 2);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	item = server_find_item_config(item_name);
	if (item == NULL) {
		printf("script warning: item %s is undefined\n", item_name);
		script_cancel(L, player_id);
		return 0;
	}

	result = player_inv_worn(p, item);
	lua_pushboolean(L, result);
	return 1;
}

static int
script_boundaryteleport(lua_State *L)
{
	lua_Integer player_id, x, y, dir;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	x = script_checkinteger(L, 2);
	y = script_checkinteger(L, 3);
	dir = script_checkinteger(L, 4);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	switch (dir) {
	case BOUND_DIR_VERT:
		if (p->mob.x == x && p->mob.y == y) {
			p->mob.y--;
		} else {
			p->mob.y++;
		}
		break;
	case BOUND_DIR_HORIZ:
		if (p->mob.x == x && p->mob.y == y) {
			p->mob.x--;
		} else {
			p->mob.x++;
		}
		break;
	case BOUND_DIR_DIAG_NW_SE:
		/* TODO */
		break;
	case BOUND_DIR_DIAG_NE_SW:
		/* TODO */
		break;
	}

	p->teleported = true;

	/* doesn't seem necessary but it's what the official server did */
	player_send_plane_init(p);
	return 0;
}

static int
script_changelevelup(lua_State *L)
{
	lua_Integer player_id;
	struct player *p;

	player_id = script_checkinteger(L, 1);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	if (p->mob.y < (PLANE_LEVEL_INC * 3)) {
		p->mob.y += PLANE_LEVEL_INC;
	} else {
		p->mob.y -= (PLANE_LEVEL_INC * 3);
	}

	p->teleported = true;
	player_send_plane_init(p);
	return 0;
}

static int
script_upstairs(lua_State *L)
{
	lua_Integer player_id, x, y;
	struct player *p;
	struct loc *loc;

	player_id = script_checkinteger(L, 1);
	x = script_checkinteger(L, 2);
	y = script_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	if (p->mob.y < (PLANE_LEVEL_INC * 3)) {
		y += PLANE_LEVEL_INC;
	} else {
		y -= (PLANE_LEVEL_INC * 3);
	}

	loc = server_find_loc(x, y);
	if (loc == NULL) {
		return 0;
	}

	/* XXX: don't hardcode directions */
	switch (loc->dir) {
	case 0:
		y += 3;
		break;
	case 4:
		y--;
		break;
	case 6:
		x--;
		break;
	}

	p->mob.x = x;
	p->mob.y = y;
	p->teleported = true;
	player_send_plane_init(p);
	return 0;
}

static int
script_changeleveldown(lua_State *L)
{
	lua_Integer player_id;
	struct player *p;

	player_id = script_checkinteger(L, 1);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	if (p->mob.y > PLANE_LEVEL_INC) {
		p->mob.y -= PLANE_LEVEL_INC;
	} else {
		p->mob.y += (PLANE_LEVEL_INC * 3);
	}

	p->teleported = true;
	player_send_plane_init(p);
	return 0;
}

static int
script_downstairs(lua_State *L)
{
	lua_Integer player_id, x, y;
	struct player *p;
	struct loc *loc;

	player_id = script_checkinteger(L, 1);
	x = script_checkinteger(L, 2);
	y = script_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	if (p->mob.y > PLANE_LEVEL_INC) {
		y -= PLANE_LEVEL_INC;
	} else {
		y += (PLANE_LEVEL_INC * 3);
	}

	loc = server_find_loc(x, y);
	if (loc == NULL) {
		return 0;
	}

	/* XXX: don't hardcode directions */
	switch (loc->dir) {
	case 0:
		y--;
		break;
	case 4:
		y += 3;
		break;
	case 6:
		x += 3;
		break;
	}

	p->mob.x = x;
	p->mob.y = y;
	p->teleported = true;
	player_send_plane_init(p);
	return 0;
}

static int
script_openshop(lua_State *L)
{
	lua_Integer player_id;
	const char *shop_name;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	shop_name = script_checkstring(L, 2);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	player_send_shop(p, shop_name);
	return 0;
}

static int
script_addnpc(lua_State *L)
{
	const char *npc_name;
	lua_Integer x, y;
	struct npc_config *npc_config;
	int result;

	npc_name = script_checkstring(L, 1);
	x = script_checkinteger(L, 2);
	y = script_checkinteger(L, 3);

	npc_config = server_find_npc_config(npc_name);
	if (npc_config == NULL) {
		printf("script warning: npc config \"%s\" not found\n", npc_name);
		return 0;
	}

	result = server_add_npc(npc_config->id, x, y);
	if (result < 0) {
		printf("script warning: unable to add npc, server_add_npc failed\n");
		return 0;
	}

	lua_pushinteger(L, result);
	return 1;
}

static int
script_playercoord(lua_State *L)
{
	lua_Integer player_id;
	struct player *p;

	player_id = script_checkinteger(L, 1);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	lua_pushinteger(L, p->mob.x);
	lua_pushinteger(L, p->mob.y);
	return 2;
}


static int
script_teleport(lua_State *L)
{
	lua_Integer player_id, x, y;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	x = script_checkinteger(L, 2);
	y = script_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	/* TODO: send teleport effect */
	p->mob.x = x;
	p->mob.y = y;
	p->teleported = true;
	return 0;
}

static int
script_qp(lua_State *L)
{
	lua_Integer player_id;
	struct player *p;

	player_id = script_checkinteger(L, 1);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	lua_pushinteger(L, p->quest_points);
	return 1;
}

static int
script_giveqp(lua_State *L)
{
	char mes[64];
	lua_Integer player_id, amount;
	struct player *p;

	player_id = script_checkinteger(L, 1);
	amount = script_checkinteger(L, 2);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	/* real authentic typo. sigh. */
	if (amount == 1) {
		(void)snprintf(mes, sizeof(mes),
		    "@gre@You haved gained 1 quest point!");
	} else {
		(void)snprintf(mes, sizeof(mes),
		    "@gre@You haved gained %d quest points!", (int)amount);
	}

	player_send_message(p, mes);
	player_send_quests(p);

	p->quest_points += amount;
	p->stats_changed = true;
	return 0;
}

static int
script_thinkbubble(lua_State *L)
{
	lua_Integer player_id;
	const char *item_name;
	struct player *p;
	struct item_config *item;

	player_id = script_checkinteger(L, 1);
	item_name = script_checkstring(L, 2);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	item = server_find_item_config(item_name);
	if (item == NULL) {
		printf("script warning: item %s is undefined\n", item_name);
		script_cancel(L, player_id);
		return 0;
	}

	p->bubble_id = item->id;
	return 0;
}

static int
script_changebound(lua_State *L)
{
	lua_Integer x, y, dir;
	const char *name;
	struct bound_config *config;
	struct bound *b;

	x = script_checkinteger(L, 1);
	y = script_checkinteger(L, 2);
	dir = script_checkinteger(L, 3);
	name = script_checkstring(L, 4);

	config = server_find_bound_config(name);
	if (config == NULL) {
		printf("script warning: bound %s is undefined\n", name);
		return 0;
	}

	b = server_find_bound(x, y, dir);
	if (b == NULL) {
		printf("script warning: couldn't find bound at %lld %lld\n", x, y);
		return 0;
	}

	b->id = config->id;

	server_add_bound(serv, b);
	return 0;
}

static int
script_changeloc(lua_State *L)
{
	lua_Integer x, y;
	const char *name;
	struct loc_config *config;
	struct loc *loc;

	x = script_checkinteger(L, 1);
	y = script_checkinteger(L, 2);
	name = script_checkstring(L, 3);

	loc = server_find_loc(x, y);
	if (loc == NULL) {
		printf("script warning: couldn't find loc at %lld %lld\n", x, y);
		return 0;
	}

	config = server_find_loc_config(name);
	if (config == NULL) {
		printf("script warning: loc %s is undefined\n", name);
		return 0;
	}

	loc->id = config->id;

	server_add_loc(serv, loc);
	return 0;
}

static int
script_restoreloc(lua_State *L)
{
	lua_Integer x, y;
	struct loc *loc;

	x = script_checkinteger(L, 1);
	y = script_checkinteger(L, 2);

	loc = server_find_loc(x, y);
	if (loc == NULL) {
		printf("script warning: couldn't find loc at %lld %lld\n", x, y);
		return 0;
	}

	loc->id = loc->orig_id;

	server_add_loc(serv, loc);
	return 0;
}

static int
script_shootplayer(lua_State *L)
{
	lua_Integer player_id, target_id;
	const char *proj_name;
	struct player *p, *target;
	struct projectile_config *proj;

	player_id = script_checkinteger(L, 1);
	target_id = script_checkinteger(L, 2);
	proj_name = script_checkstring(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	target = id_to_player(target_id);
	if (target == NULL) {
		printf("script warning: player %lld is undefined\n", target_id);
		script_cancel(L, target_id);
		return 0;
	}

	proj = server_find_projectile(proj_name);
	if (proj == NULL) {
		printf("script warning: projectile %s is undefined\n", proj_name);
		script_cancel(L, player_id);
		return 0;
	}

	player_shoot_pvp(p, proj, target);
	return 0;
}

static int
script_shootnpc(lua_State *L)
{
	lua_Integer player_id, target_id;
	const char *proj_name;
	struct player *p;
	struct npc *target;
	struct projectile_config *proj;

	player_id = script_checkinteger(L, 1);
	target_id = script_checkinteger(L, 2);
	proj_name = script_checkstring(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %lld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	target = id_to_npc(target_id);
	if (target == NULL) {
		printf("script warning: npc %lld is undefined\n", target_id);
		script_cancel(L, target_id);
		return 0;
	}

	proj = server_find_projectile(proj_name);
	if (proj == NULL) {
		printf("script warning: projectile %s is undefined\n", proj_name);
		script_cancel(L, player_id);
		return 0;
	}

	player_shoot_pvm(p, proj, target);
	return 0;
}

void
script_tick(lua_State *L)
{
	assert(L != NULL);
	lua_getglobal(L, "script_engine_tick");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_tick");
		return;
	}
	safe_call(L, 0, 0, -1);
}

void
script_process(lua_State *L, struct player *p)
{
	assert(L != NULL);
	assert(p != NULL);
	lua_getglobal(L, "script_engine_process");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_process");
		return;
	}
	lua_pushnumber(L, p->mob.id);
	safe_call(L, 1, 0, p->mob.id);
}

void
script_onnpctalk(lua_State *L, struct player *p, struct npc *npc)
{
	char mes[128];
	int result = 0;

	for (size_t i = 0; i < npc->config->name_count; ++i) {
		lua_getglobal(L, "script_engine_talknpc");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_talknpc");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, npc->config->names[i]);
		lua_pushnumber(L, npc->mob.id);
		safe_call(L, 3, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}
	(void)snprintf(mes, sizeof(mes),
	    "The %s does not appear interested in talking",
	    npc->config->names[0]);
	player_send_message(p, mes);
}

void
script_onopinv(lua_State *L, struct player *p, struct item_config *item)
{
	int result = 0;

	for (size_t i = 0; i < item->name_count; ++i) {
		lua_getglobal(L, "script_engine_opinv");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_opinv");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, item->names[i]);
		safe_call(L, 2, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}
	player_send_message(p, "Nothing interesting happens");
}

/* XXX: this name is assumed, not actually found in runescript docs */
void
script_onskillplayer(lua_State *L, struct player *p,
    struct player *target, struct spell_config *spell)
{
	lua_getglobal(L, "script_engine_skillplayer");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_skillplayer");
		return;
	}
	lua_pushnumber(L, p->mob.id);
	lua_pushnumber(L, target->mob.id);
	lua_pushstring(L, spell->name);
	safe_call(L, 3, 0, p->mob.id);
}

void
script_onuseobj(lua_State *L, struct player *p,
    struct item_config *item, int x, int y, struct item_config *invitem)
{
	bool result;

	for (size_t i = 0; i < invitem->name_count; ++i) {
		for (size_t j = 0; j < item->name_count; ++j) {
			lua_getglobal(L, "script_engine_useobj");
			if (!lua_isfunction(L, -1)) {
				puts("script error: can't find essential function script_engine_useobj");
				return;
			}
			lua_pushnumber(L, p->mob.id);
			lua_pushstring(L, item->names[j]);
			lua_pushnumber(L, x);
			lua_pushnumber(L, y);
			lua_pushstring(L, invitem->names[i]);
			safe_call(L, 5, 1, p->mob.id);
			result = lua_toboolean(L, -1);
			if (result != 0) {
				return;
			}
		}
	}

	player_send_message(p, "Nothing interesting happens");
}

void
script_onuseinv(lua_State *L, struct player *p,
    struct item_config *item1, struct item_config *item2)
{
	bool result;

	for (size_t i = 0; i < item1->name_count; ++i) {
		for (size_t j = 0; j < item2->name_count; ++j) {
			lua_getglobal(L, "script_engine_useinv");
			if (!lua_isfunction(L, -1)) {
				puts("script error: can't find essential function script_engine_useinv");
				return;
			}
			lua_pushnumber(L, p->mob.id);
			lua_pushstring(L, item1->names[i]);
			lua_pushstring(L, item2->names[j]);
			safe_call(L, 3, 1, p->mob.id);
			result = lua_toboolean(L, -1);
			if (result != 0) {
				return;
			}
		}
	}

	player_send_message(p, "Nothing interesting happens");
}

void
script_onspellinv(lua_State *L, struct player *p,
    struct item_config *item)
{
	bool result;

	assert(item != NULL);
	assert(p->spell != NULL);

	for (size_t i = 0; i < item->name_count; ++i) {
		lua_getglobal(L, "script_engine_spellinv");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_spellinv");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, item->names[i]);
		lua_pushstring(L, p->spell->name);
		safe_call(L, 3, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}

	player_send_message(p, "Nothing interesting happens");
}

void
script_onusenpc(lua_State *L, struct player *p,
    struct npc *npc, struct item_config *item)
{
	bool result;

	for (size_t i = 0; i < npc->config->name_count; ++i) {
		for (size_t j = 0; j < item->name_count; ++j) {
			lua_getglobal(L, "script_engine_usenpc");
			if (!lua_isfunction(L, -1)) {
				puts("script error: can't find essential function script_engine_usenpc");
				return;
			}
			lua_pushnumber(L, p->mob.id);
			lua_pushnumber(L, npc->mob.id);
			lua_pushstring(L, npc->config->names[i]);
			lua_pushstring(L, item->names[j]);
			safe_call(L, 4, 1, p->mob.id);
			result = lua_toboolean(L, -1);
			/*
			 * replay:
			 * rsc-preservation.xyz/Quests/sheep-shearer-zezima
			 * has the sheep facing the player even on failure
			 */
			if (result != 0) {
				mob_face(&npc->mob, p->mob.x, p->mob.y);
				mob_face(&p->mob, npc->mob.x, npc->mob.y);
				return;
			}
		}
	}

	player_send_message(p, "Nothing interesting happens");
}

void
script_onuseloc(lua_State *L, struct player *p,
    struct loc *loc, struct item_config *item)
{
	struct loc_config *config;
	bool result;

	config = server_loc_config_by_id(loc->id);
	assert(config != NULL);

	for (size_t i = 0; i < config->name_count; ++i) {
		for (size_t j = 0; j < item->name_count; ++j) {
			lua_getglobal(L, "script_engine_useloc");
			if (!lua_isfunction(L, -1)) {
				puts("script error: can't find essential function script_engine_useloc");
				return;
			}
			lua_pushnumber(L, p->mob.id);
			lua_pushstring(L, config->names[i]);
			lua_pushnumber(L, loc->x);
			lua_pushnumber(L, loc->y);
			lua_pushstring(L, item->names[j]);
			safe_call(L, 5, 1, p->mob.id);
			result = lua_toboolean(L, -1);
			if (result != 0) {
				return;
			}
		}
	}

	player_send_message(p, "Nothing interesting happens");
}

void
script_onusebound(lua_State *L, struct player *p,
    struct bound *bound, struct item_config *item)
{
	struct bound_config *config;
	bool result;

	config = server_bound_config_by_id(bound->id);
	assert(config != NULL);

	for (size_t i = 0; i < config->name_count; ++i) {
		for (size_t j = 0; j < item->name_count; ++j) {
			lua_getglobal(L, "script_engine_usebound");
			if (!lua_isfunction(L, -1)) {
				puts("script error: can't find essential function script_engine_usebound");
				return;
			}
			lua_pushnumber(L, p->mob.id);
			lua_pushstring(L, config->names[i]);
			lua_pushnumber(L, bound->x);
			lua_pushnumber(L, bound->y);
			lua_pushnumber(L, bound->dir);
			lua_pushstring(L, item->names[j]);
			safe_call(L, 6, 1, p->mob.id);
			result = lua_toboolean(L, -1);
			if (result != 0) {
				return;
			}
		}
	}

	player_send_message(p, "Nothing interesting happens");
}

void
script_onskillnpc(lua_State *L, struct player *p,
    struct npc *npc, struct spell_config *spell)
{
	bool result;

	for (size_t i = 0; i < npc->config->name_count; ++i) {
		lua_getglobal(L, "script_engine_skillnpc");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_skillnpc");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, npc->config->names[i]);
		lua_pushnumber(L, npc->mob.id);
		lua_pushstring(L, spell->name);
		safe_call(L, 4, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}

	lua_getglobal(L, "script_engine_skillnpc");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_skillnpc");
		return;
	}
	lua_pushnumber(L, p->mob.id);
	lua_pushstring(L, "_");
	lua_pushnumber(L, npc->mob.id);
	lua_pushstring(L, spell->name);
	safe_call(L, 4, 1, p->mob.id);
	lua_pop(L, -1);
}

void
script_onopbound1(lua_State *L, struct player *p, struct bound *bound)
{
	bool result;
	struct bound_config *config;

	config = server_bound_config_by_id(bound->id);
	assert(config != NULL);

	for (size_t i = 0; i < config->name_count; ++i) {
		lua_getglobal(L, "script_engine_opbound1");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_opbound1");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, config->names[i]);
		lua_pushnumber(L, bound->x);
		lua_pushnumber(L, bound->y);
		lua_pushnumber(L, bound->dir);
		safe_call(L, 5, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}
	player_send_message(p, "Nothing interesting happens");
}

void
script_onopbound2(lua_State *L, struct player *p, struct bound *bound)
{
	bool result;
	struct bound_config *config;

	config = server_bound_config_by_id(bound->id);
	assert(config != NULL);

	for (size_t i = 0; i < config->name_count; ++i) {
		lua_getglobal(L, "script_engine_opbound2");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_opbound2");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, config->names[i]);
		lua_pushnumber(L, bound->x);
		lua_pushnumber(L, bound->y);
		lua_pushnumber(L, bound->dir);
		safe_call(L, 5, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}
	player_send_message(p, "Nothing interesting happens");
}

void
script_onoploc1(lua_State *L, struct player *p, struct loc *loc)
{
	bool result;
	struct loc_config *config;

	config = server_loc_config_by_id(loc->id);
	assert(config != NULL);

	for (size_t i = 0; i < config->name_count; ++i) {
		lua_getglobal(L, "script_engine_oploc1");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_oploc1");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, config->names[i]);
		lua_pushnumber(L, loc->x);
		lua_pushnumber(L, loc->y);
		safe_call(L, 4, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}
	player_send_message(p, "Nothing interesting happens");
}

void
script_onoploc2(lua_State *L, struct player *p, struct loc *loc)
{
	bool result;
	struct loc_config *config;

	config = server_loc_config_by_id(loc->id);
	assert(config != NULL);

	for (size_t i = 0; i < config->name_count; ++i) {
		lua_getglobal(L, "script_engine_oploc2");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_oploc2");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, config->names[i]);
		lua_pushnumber(L, loc->x);
		lua_pushnumber(L, loc->y);
		safe_call(L, 4, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}
	player_send_message(p, "Nothing interesting happens");
}

void
script_onattacknpc(lua_State *L, struct player *p, struct npc *npc)
{
	bool result = false;

	for (size_t i = 0; i < npc->config->name_count; ++i) {
		lua_getglobal(L, "script_engine_attacknpc");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_attacknpc");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushnumber(L, npc->mob.id);
		lua_pushstring(L, npc->config->names[i]);
		safe_call(L, 3, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return;
		}
	}
}

bool
script_onkillnpc(lua_State *L, struct player *p, struct npc *npc)
{
	bool result;

	for (size_t i = 0; i < npc->config->name_count; ++i) {
		lua_getglobal(L, "script_engine_killnpc");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_killnpc");
			return false;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushnumber(L, npc->mob.id);
		lua_pushstring(L, npc->config->names[i]);
		lua_pushnumber(L, npc->mob.x);
		lua_pushnumber(L, npc->mob.y);
		safe_call(L, 5, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return true;
		}
	}
	return false;
}

bool
script_onwearobj(lua_State *L, struct player *p, struct item_config *item)
{
	bool result;

	assert(p != NULL);
	assert(item != NULL);

	for (size_t i = 0; i < item->name_count; ++i) {
		lua_getglobal(L, "script_engine_wearobj");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_wearobj");
			return true;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, item->names[i]);
		safe_call(L, 2, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return false;
		}
	}
	return true;
}

bool
script_ontakeobj(lua_State *L, struct player *p, struct ground_item *item)
{
	struct item_config *config;
	bool result;

	config = server_item_config_by_id(item->id);
	assert(config != NULL);

	for (size_t i = 0; i < config->name_count; ++i) {
		lua_getglobal(L, "script_engine_takeobj");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_takeobj");
			return false;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, config->names[i]);
		lua_pushnumber(L, item->x);
		lua_pushnumber(L, item->y);
		safe_call(L, 4, 1, p->mob.id);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			return true;
		}
	}
	return false;
}

void
script_cancel(lua_State *L, uint16_t player_id)
{
	lua_getglobal(L, "script_engine_cancel");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_cancel");
		return;
	}
	lua_pushnumber(L, player_id);
	lua_pcall(L, 1, 0, 0);
}

void
script_multi_answer(lua_State *L, struct player *p, int option)
{
	lua_getglobal(L, "script_engine_answer");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_answer");
		return;
	}
	lua_pushnumber(L, p->mob.id);
	lua_pushnumber(L, option + 1);
	safe_call(L, 2, 0, p->mob.id);
}

lua_State *
script_init(struct server *s)
{
	lua_State *L;

	serv = s;

	L = luaL_newstate();
	luaL_openlibs(L);

	lua_atpanic(L, script_panic);

	lua_pushcfunction(L, script_say);
	lua_setglobal(L, "_say");

	lua_pushcfunction(L, script_addloc);
	lua_setglobal(L, "_addloc");

	lua_pushcfunction(L, script_delloc);
	lua_setglobal(L, "_delloc");

	lua_pushcfunction(L, script_npcsay);
	lua_setglobal(L, "_npcsay");

	lua_pushcfunction(L, script_multi);
	lua_setglobal(L, "_multi");

	lua_pushcfunction(L, script_blocked);
	lua_setglobal(L, "blocked");

	lua_pushcfunction(L, script_npcattack);
	lua_setglobal(L, "npcattack");

	lua_pushcfunction(L, script_npcbusy);
	lua_setglobal(L, "npcbusy");

	lua_pushcfunction(L, script_npcunbusy);
	lua_setglobal(L, "npcunbusy");

	lua_pushcfunction(L, script_playerbusy);
	lua_setglobal(L, "playerbusy");

	lua_pushcfunction(L, script_playerunbusy);
	lua_setglobal(L, "playerunbusy");

	lua_pushcfunction(L, script_mes);
	lua_setglobal(L, "mes");

	lua_pushcfunction(L, script_give);
	lua_setglobal(L, "give");

	lua_pushcfunction(L, script_remove);
	lua_setglobal(L, "remove");

	lua_pushcfunction(L, script_sellinv);
	lua_setglobal(L, "sellinv");

	lua_pushcfunction(L, script_held);
	lua_setglobal(L, "held");

	lua_pushcfunction(L, script_worn);
	lua_setglobal(L, "worn");

	lua_pushcfunction(L, script_random);
	lua_setglobal(L, "random");

	lua_pushcfunction(L, script_randomvar);
	lua_setglobal(L, "randomvar");

	lua_pushcfunction(L, script_advancestat);
	lua_setglobal(L, "advancestat");

	lua_pushcfunction(L, script_healstat);
	lua_setglobal(L, "healstat");

	lua_pushcfunction(L, script_statatleast);
	lua_setglobal(L, "statatleast");

	lua_pushcfunction(L, script_statup);
	lua_setglobal(L, "statup");

	lua_pushcfunction(L, script_statdown);
	lua_setglobal(L, "statdown");

	lua_pushcfunction(L, script_statrandom);
	lua_setglobal(L, "statrandom");

	lua_pushcfunction(L, script_npcstatup);
	lua_setglobal(L, "npcstatup");

	lua_pushcfunction(L, script_npcstatdown);
	lua_setglobal(L, "npcstatdown");

	lua_pushcfunction(L, script_addstat);
	lua_setglobal(L, "addstat");

	lua_pushcfunction(L, script_substat);
	lua_setglobal(L, "substat");

	lua_pushcfunction(L, script_npcaddstat);
	lua_setglobal(L, "npcaddstat");

	lua_pushcfunction(L, script_npcsubstat);
	lua_setglobal(L, "npcsubstat");

	lua_pushcfunction(L, script_npchealstat);
	lua_setglobal(L, "npchealstat");

	lua_pushcfunction(L, script_thinkbubble);
	lua_setglobal(L, "thinkbubble");

	lua_pushcfunction(L, script_boundaryteleport);
	lua_setglobal(L, "boundaryteleport");

	lua_pushcfunction(L, script_upstairs);
	lua_setglobal(L, "upstairs");

	lua_pushcfunction(L, script_downstairs);
	lua_setglobal(L, "downstairs");

	lua_pushcfunction(L, script_changelevelup);
	lua_setglobal(L, "changelevelup");

	lua_pushcfunction(L, script_changeleveldown);
	lua_setglobal(L, "changeleveldown");

	lua_pushcfunction(L, script_openshop);
	lua_setglobal(L, "openshop");

	lua_pushcfunction(L, script_giveqp);
	lua_setglobal(L, "giveqp");

	lua_pushcfunction(L, script_shootplayer);
	lua_setglobal(L, "shootplayer");

	lua_pushcfunction(L, script_shootnpc);
	lua_setglobal(L, "shootnpc");

	lua_pushcfunction(L, script_changebound);
	lua_setglobal(L, "changebound");

	lua_pushcfunction(L, script_changeloc);
	lua_setglobal(L, "changeloc");

	lua_pushcfunction(L, script_restoreloc);
	lua_setglobal(L, "_restoreloc");

	lua_pushcfunction(L, script_displaybalance);
	lua_setglobal(L, "displaybalance");

	lua_pushcfunction(L, script_male);
	lua_setglobal(L, "male");

	lua_pushcfunction(L, script_addobject);
	lua_setglobal(L, "addobject");

	lua_pushcfunction(L, script_delobject);
	lua_setglobal(L, "delobject");

	lua_pushcfunction(L, script_takeobject);
	lua_setglobal(L, "takeobject");

	lua_pushcfunction(L, script_getvar);
	lua_setglobal(L, "getvar");

	lua_pushcfunction(L, script_setvar);
	lua_setglobal(L, "setvar");

	lua_pushcfunction(L, script_addnpc);
	lua_setglobal(L, "addnpc");

	lua_pushcfunction(L, script_playercoord);
	lua_setglobal(L, "playercoord");

	lua_pushcfunction(L, script_teleport);
	lua_setglobal(L, "teleport");

	lua_pushcfunction(L, script_nearnpc);
	lua_setglobal(L, "nearnpc");

	lua_pushcfunction(L, script_qp);
	lua_setglobal(L, "qp");

	if (luaL_dofile(L, "./lua/script.lua") != LUA_OK) {
		printf("script error %s:\n",  lua_tostring(L, -1));
	}

	return L;
}

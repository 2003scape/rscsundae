#include <assert.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include "ext/flea.h"
#include "script.h"
#include "server.h"

static struct server *serv;
static int script_say(lua_State *);
static int script_npcattack(lua_State *);
static int script_npcsay(lua_State *);
static int script_random(lua_State *);
static int script_give(lua_State *);

static int
script_say(lua_State *L)
{
	lua_Integer id = luaL_checkinteger(L, 1);
	const char *mes = luaL_checkstring(L, 2);

	printf("say %s\n", mes);

	return 0;
}

static int
script_npcattack(lua_State *L)
{
	lua_Integer npc_id = luaL_checkinteger(L, 1);

	printf("npcattack\n");

	return 0;
}

static int
script_npcsay(lua_State *L)
{
	lua_Integer npc_id = luaL_checkinteger(L, 1);
	const char *mes = luaL_checkstring(L, 2);

	printf("npcsay %s\n", mes);

	return 0;
}

static int
script_random(lua_State *L)
{
	lua_Integer probability;
	lua_Integer result;

	probability = luaL_checkinteger(L, 1);
	result = (int)((ranval(&serv->ran) / (double)UINT32_MAX) * 255.0);
	lua_pushboolean(L, result < probability);
	return 1;
}

static int
script_mes(lua_State *L)
{
	lua_Integer player_id;
	const char *mes;
	struct player *p;

	player_id = luaL_checkinteger(L, 1);
	mes = luaL_checkstring(L, 2);
	if (player_id < 0 || player_id >= MAXPLAYERS) {
		printf("script warning: played id %d out of range\n", player_id);
		return 0;
	}
	p = serv->players[player_id];
	if (p == NULL) {
		/* TODO should cancel script here */
		printf("script warning: player %d is undefined\n", player_id);
		return 0;
	}
	player_send_message(p, mes);
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

	player_id = luaL_checkinteger(L, 1);
	item_name = luaL_checkstring(L, 2);
	amount = luaL_checkinteger(L, 3);

	if (player_id < 0 || player_id >= MAXPLAYERS) {
		printf("script warning: played id %d out of range\n", player_id);
		return 0;
	}
	p = serv->players[player_id];
	if (p == NULL) {
		/* TODO should cancel script here */
		printf("script warning: player %d is undefined\n", player_id);
		return 0;
	}

	item = server_find_item_config(item_name);
	if (item == NULL) {
		/* TODO should cancel script here */
		printf("script warning: item %s is undefined\n", item_name);
		return 0;
	}

	player_inv_give(player_id, item, amount);
	return 0;
}

static int
script_default_talk(lua_State *L)
{
	puts("The avocado doesn't seem interested in talking.");
	return 0;
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
	lua_pcall(L, 1, 0, 0);
}

void
script_onnpctalk(lua_State *L, struct player *p, struct npc *npc)
{
	lua_getglobal(L, "script_engine_ontalknpc");
	if (!lua_isfunction(L, -1)) { 
		puts("script error: can't find essential function script_engine_ontalknpc");
		return;
	}
	lua_pushnumber(L, p->mob.id);
	lua_pushstring(L, "man1");
	lua_pushnumber(L, npc->mob.id);
	lua_pcall(L, 3, 0, 0);
}

lua_State *
script_init(struct server *s)
{
	lua_State *L;

	serv = s;

	L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, script_say);
	lua_setglobal(L, "_say");

	lua_pushcfunction(L, script_npcsay);
	lua_setglobal(L, "_npcsay");

	lua_pushcfunction(L, script_npcattack);
	lua_setglobal(L, "npcattack");

	lua_pushcfunction(L, script_mes);
	lua_setglobal(L, "mes");

	lua_pushcfunction(L, script_give);
	lua_setglobal(L, "give");

	lua_pushcfunction(L, script_random);
	lua_setglobal(L, "random");

	lua_pushcfunction(L, script_default_talk);
	lua_setglobal(L, "_default_talk");

	/* TODO: configurable path */
	if (luaL_dofile(L, "./src/lua/script.lua") != LUA_OK) {
		printf("script error %s:\n",  lua_tostring(L, -1));
	}

	return L;
}

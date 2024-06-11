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

static struct server *serv;
static int script_say(lua_State *);
static int script_npcattack(lua_State *);
static int script_npcsay(lua_State *);
static int script_random(lua_State *);
static int script_give(lua_State *);
static int script_remove(lua_State *);
static int script_advancestat(lua_State *);
static int script_healstat(lua_State *);
static int script_addstat(lua_State *);
static int script_substat(lua_State *);
static int script_statup(lua_State *);
static int script_statdown(lua_State *);
static int script_thinkbubble(lua_State *);
static int script_default_talk(lua_State *);
static struct player *id_to_player(lua_Integer);
static struct npc *id_to_npc(lua_Integer);

static struct player *
id_to_player(lua_Integer id)
{
	if (id < 0 || id >= MAXPLAYERS) {
		printf("script warning: player id %ld out of range\n", id);
		return NULL;
	}
	return serv->players[id];
}

static struct npc *
id_to_npc(lua_Integer id)
{
	if (id < 0 || id >= MAXNPCS) {
		printf("script warning: npc id %ld out of range\n", id);
		return NULL;
	}
	return serv->npcs[id];
}

static int
script_say(lua_State *L)
{
	lua_Integer id = luaL_checkinteger(L, 1);
	const char *mes = luaL_checkstring(L, 2);
	size_t len;
	struct player *p;

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	len = strlen(mes);
	if (len > MAX_CHAT_LEN) {
		len = MAX_CHAT_LEN;
	}

	encode_chat_legacy(mes, (uint8_t *)p->mob.chat_enc, len);
	p->mob.chat_len = len;
	p->chat_type = CHAT_TYPE_QUEST;

	return 0;
}

static int
script_npcattack(lua_State *L)
{
	lua_Integer npc_id = luaL_checkinteger(L, 1);

	(void)npc_id;
	printf("npcattack\n");

	return 0;
}

static int
script_npcsay(lua_State *L)
{
	lua_Integer id = luaL_checkinteger(L, 1);
	const char *mes = luaL_checkstring(L, 2);
	size_t len;
	struct npc *npc;

	/* TODO: player should face NPC */

	npc = id_to_npc(id);
	if (npc == NULL) {
		printf("script warning: player %ld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	len = strlen(mes);
	if (len > MAX_CHAT_LEN) {
		len = MAX_CHAT_LEN;
	}

	encode_chat_legacy(mes, (uint8_t *)npc->mob.chat_enc, len);
	npc->mob.chat_len = len;

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
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
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

	player_id = luaL_checkinteger(L, 1);
	stat = luaL_checkinteger(L, 2);
	base = luaL_checkinteger(L, 3);
	exp = luaL_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %ld\n", stat);
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

	player_id = luaL_checkinteger(L, 1);
	stat = luaL_checkinteger(L, 2);
	constant = luaL_checkinteger(L, 3);
	percent = luaL_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %ld\n", stat);
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

	player_id = luaL_checkinteger(L, 1);
	stat = luaL_checkinteger(L, 2);
	constant = luaL_checkinteger(L, 3);
	percent = luaL_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %ld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	stat_add(&p->mob, stat, constant, percent);
	player_send_stat(p, stat);
	return 0;
}

static int
script_statup(lua_State *L)
{
	lua_Integer player_id, stat;
	struct player *p;
	int b;

	player_id = luaL_checkinteger(L, 1);
	stat = luaL_checkinteger(L, 2);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %ld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	b = stat_up(&p->mob, stat);
	lua_pushboolean(L, b);
	return 0;
}

static int
script_statdown(lua_State *L)
{
	lua_Integer player_id, stat;
	struct player *p;
	int b;

	player_id = luaL_checkinteger(L, 1);
	stat = luaL_checkinteger(L, 2);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %ld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	b = stat_down(&p->mob, stat);
	lua_pushboolean(L, b);
	return 0;
}

static int
script_substat(lua_State *L)
{
	lua_Integer player_id;
	lua_Integer stat, constant, percent;
	struct player *p;

	player_id = luaL_checkinteger(L, 1);
	stat = luaL_checkinteger(L, 2);
	constant = luaL_checkinteger(L, 3);
	percent = luaL_checkinteger(L, 4);
	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}
	if (stat < 0 || stat >= MAX_SKILL_ID) {
		printf("script warning: invalid stat id %ld\n", stat);
		script_cancel(L, player_id);
		return 0;
	}
	stat_remove(&p->mob, stat, constant, percent);
	player_send_stat(p, stat);
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

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
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

	player_id = luaL_checkinteger(L, 1);
	item_name = luaL_checkstring(L, 2);
	amount = luaL_checkinteger(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
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
script_default_talk(lua_State *L)
{
	(void)L;

	/* TODO */
	puts("The avocado does not appear interested in talking");
	return 0;
}

static int
script_thinkbubble(lua_State *L)
{
	lua_Integer player_id;
	const char *item_name;
	struct player *p;
	struct item_config *item;

	player_id = luaL_checkinteger(L, 1);
	item_name = luaL_checkstring(L, 2);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
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

void
script_onuseobj(lua_State *L, struct player *p, const char *name)
{
	lua_getglobal(L, "script_engine_onuseobj");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_onuseobj");
		return;
	}
	lua_pushnumber(L, p->mob.id);
	lua_pushstring(L, name);
	lua_pcall(L, 2, 0, 0);
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

	lua_pushcfunction(L, script_remove);
	lua_setglobal(L, "remove");

	lua_pushcfunction(L, script_random);
	lua_setglobal(L, "random");

	lua_pushcfunction(L, script_advancestat);
	lua_setglobal(L, "advancestat");

	lua_pushcfunction(L, script_healstat);
	lua_setglobal(L, "healstat");

	lua_pushcfunction(L, script_statup);
	lua_setglobal(L, "statup");

	lua_pushcfunction(L, script_statdown);
	lua_setglobal(L, "statdown");

	lua_pushcfunction(L, script_addstat);
	lua_setglobal(L, "addstat");

	lua_pushcfunction(L, script_substat);
	lua_setglobal(L, "substat");

	lua_pushcfunction(L, script_thinkbubble);
	lua_setglobal(L, "thinkbubble");

	lua_pushcfunction(L, script_default_talk);
	lua_setglobal(L, "_default_talk");

	/* TODO: configurable path */
	if (luaL_dofile(L, "./data/lua/script.lua") != LUA_OK) {
		printf("script error %s:\n",  lua_tostring(L, -1));
	}

	return L;
}

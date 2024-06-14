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
static int script_npcbusy(lua_State *);
static int script_npcunbusy(lua_State *);
static int script_npcsay(lua_State *);
static int script_random(lua_State *);
static int script_give(lua_State *);
static int script_remove(lua_State *);
static int script_held(lua_State *);
static int script_worn(lua_State *);
static int script_advancestat(lua_State *);
static int script_healstat(lua_State *);
static int script_addstat(lua_State *);
static int script_substat(lua_State *);
static int script_statup(lua_State *);
static int script_statdown(lua_State *);
static int script_thinkbubble(lua_State *);
static int script_shootplayer(lua_State *);
static int script_multi(lua_State *);
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
	p->mob.chat_len = len;
	p->chat_type = CHAT_TYPE_QUEST;

	return 0;
}

static int
script_multi(lua_State *L)
{
	lua_Integer id = luaL_checkinteger(L, 1);
	const char *options[MAX_MULTI_SIZE];
	struct player *p;
	uint8_t option_count = 0;

	p = id_to_player(id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	for (int i = 1; i < (MAX_MULTI_SIZE + 1); ++i) {
		lua_geti(L, 2, i);
		if (!lua_isstring(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		options[i - 1] = luaL_checkstring(L, -1);
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
	lua_Integer npc_id = luaL_checkinteger(L, 1);

	(void)npc_id;
	printf("npcattack\n");

	return 0;
}

static int
script_npcbusy(lua_State *L)
{
	lua_Integer id = luaL_checkinteger(L, 1);
	struct npc *npc;

	npc = id_to_npc(id);
	if (npc == NULL) {
		printf("script warning: npc %ld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	npc->busy = true;
	return 0;
}

static int
script_npcunbusy(lua_State *L)
{
	lua_Integer id = luaL_checkinteger(L, 1);
	struct npc *npc;

	npc = id_to_npc(id);
	if (npc == NULL) {
		printf("script warning: npc %ld is undefined\n", id);
		script_cancel(L, id);
		return 0;
	}

	npc->busy = false;
	return 0;
}

static int
script_npcsay(lua_State *L)
{
	lua_Integer id = luaL_checkinteger(L, 1);
	const char *mes = luaL_checkstring(L, 2);
	size_t len;
	struct npc *npc;
	struct player *player;

	npc = id_to_npc(id);
	if (npc == NULL) {
		printf("script warning: npc %ld is undefined\n", id);
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
	return 1;
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
	return 1;
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
script_held(lua_State *L)
{
	const char *item_name;
	lua_Integer amount;
	lua_Integer player_id;
	struct player *p;
	struct item_config *item;
	int result;

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

	result = player_inv_worn(p, item);
	lua_pushboolean(L, result);
	return 1;
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

static int
script_shootplayer(lua_State *L)
{
	lua_Integer player_id, target_id;
	const char *proj_name;
	struct player *p, *target;
	struct projectile_config *proj;

	player_id = luaL_checkinteger(L, 1);
	target_id = luaL_checkinteger(L, 2);
	proj_name = luaL_checkstring(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	target = id_to_player(target_id);
	if (target == NULL) {
		printf("script warning: player %ld is undefined\n", target_id);
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

	player_id = luaL_checkinteger(L, 1);
	target_id = luaL_checkinteger(L, 2);
	proj_name = luaL_checkstring(L, 3);

	p = id_to_player(player_id);
	if (p == NULL) {
		printf("script warning: player %ld is undefined\n", player_id);
		script_cancel(L, player_id);
		return 0;
	}

	target = id_to_npc(target_id);
	if (target == NULL) {
		printf("script warning: npc %ld is undefined\n", target_id);
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
	char mes[128];
	int result = 0;

	for (size_t i = 0; i < npc->config->name_count; ++i) {
		lua_getglobal(L, "script_engine_ontalknpc");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_ontalknpc");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, npc->config->names[i]);
		lua_pushnumber(L, npc->mob.id);
		lua_pcall(L, 3, 1, 0);
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
script_onuseobj(lua_State *L, struct player *p, struct item_config *item)
{
	int result = 0;

	for (size_t i = 0; i < item->name_count; ++i) {
		lua_getglobal(L, "script_engine_onuseobj");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_onuseobj");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, item->names[i]);
		lua_pcall(L, 2, 1, 0);
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
	lua_getglobal(L, "script_engine_onskillplayer");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_onskillplayer");
		return;
	}
	lua_pushnumber(L, p->mob.id);
	lua_pushnumber(L, target->mob.id);
	lua_pushstring(L, spell->name);
	lua_pcall(L, 3, 0, 0);
}

void
script_onskillnpc(lua_State *L, struct player *p,
    struct npc *npc, struct spell_config *spell)
{
	bool result;

	printf("attempt to find an onskillnpc handler\n");

	for (size_t i = 0; i < npc->config->name_count; ++i) {
		lua_getglobal(L, "script_engine_onskillnpc");
		if (!lua_isfunction(L, -1)) {
			puts("script error: can't find essential function script_engine_onskillnpc");
			return;
		}
		lua_pushnumber(L, p->mob.id);
		lua_pushstring(L, npc->config->names[i]);
		lua_pushnumber(L, npc->mob.id);
		lua_pushstring(L, spell->name);
		lua_pcall(L, 4, 1, 0);
		result = lua_toboolean(L, -1);
		if (result != 0) {
			printf("found onskillnpc handler\n");
			return;
		} else {
			printf("thingy invalid\n");
		}
	}

	lua_getglobal(L, "script_engine_onskillnpc");
	if (!lua_isfunction(L, -1)) {
		puts("script error: can't find essential function script_engine_onskillnpc");
		return;
	}
	lua_pushnumber(L, p->mob.id);
	lua_pushstring(L, "_");
	lua_pushnumber(L, npc->mob.id);
	lua_pushstring(L, spell->name);
	lua_pcall(L, 4, 1, 0);
	lua_pop(L, -1);
	printf("running default onskillnpc handler\n");
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
	lua_pcall(L, 2, 0, 0);
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

	lua_pushcfunction(L, script_multi);
	lua_setglobal(L, "_multi");

	lua_pushcfunction(L, script_npcattack);
	lua_setglobal(L, "npcattack");

	lua_pushcfunction(L, script_npcbusy);
	lua_setglobal(L, "npcbusy");

	lua_pushcfunction(L, script_npcunbusy);
	lua_setglobal(L, "npcunbusy");

	lua_pushcfunction(L, script_mes);
	lua_setglobal(L, "mes");

	lua_pushcfunction(L, script_give);
	lua_setglobal(L, "give");

	lua_pushcfunction(L, script_remove);
	lua_setglobal(L, "remove");

	lua_pushcfunction(L, script_held);
	lua_setglobal(L, "held");

	lua_pushcfunction(L, script_worn);
	lua_setglobal(L, "worn");

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

	lua_pushcfunction(L, script_shootplayer);
	lua_setglobal(L, "shootplayer");

	lua_pushcfunction(L, script_shootnpc);
	lua_setglobal(L, "shootnpc");

	/* TODO: configurable path */
	if (luaL_dofile(L, "./data/lua/script.lua") != LUA_OK) {
		printf("script error %s:\n",  lua_tostring(L, -1));
	}

	return L;
}

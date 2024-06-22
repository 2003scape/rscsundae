#ifndef SCRIPT_H
#define SCRIPT_H
#include <lua.h>
#include "config/config.h"
#include "entity.h"

lua_State *script_init(struct server *);
void script_tick(lua_State *);
void script_process(lua_State *, struct player *);
void script_multi_answer(lua_State *, struct player *, int);
void script_cancel(lua_State *, uint16_t);
void script_onnpctalk(lua_State *, struct player *, struct npc *);
void script_onopinv(lua_State *, struct player *, struct item_config *);
void script_onskillplayer(lua_State *, struct player *,
    struct player *, struct spell_config *);
void script_onskillnpc(lua_State *, struct player *,
    struct npc *, struct spell_config *);
void script_onopbound1(lua_State *, struct player *, struct bound *);
void script_onopbound2(lua_State *, struct player *, struct bound *);
void script_onoploc1(lua_State *, struct player *, struct loc *);
void script_onoploc2(lua_State *, struct player *, struct loc *);
void script_onuseloc(lua_State *, struct player *, struct loc *, struct item_config *);
void script_onuseobj(lua_State *, struct player *,
    struct item_config *, int, int, struct item_config *);
void script_onuseinv(lua_State *, struct player *,
    struct item_config *, struct item_config *);
void script_onusenpc(lua_State *, struct player *,
    struct npc *, struct item_config *);
bool script_onkillnpc(lua_State *, struct player *, struct npc *);

#endif

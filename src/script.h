#ifndef SCRIPT_H
#define SCRIPT_H
#include <lua.h>
#include "config/config.h"
#include "entity.h"

lua_State *script_init(struct server *);
void script_process(lua_State *, struct player *);
void script_multi_answer(lua_State *, struct player *, int);
void script_cancel(lua_State *, uint16_t);
void script_onnpctalk(lua_State *, struct player *, struct npc *);
void script_onuseobj(lua_State *, struct player *, struct item_config *);

#endif

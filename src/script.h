#ifndef SCRIPT_H
#define SCRIPT_H
#include <lua.h>
#include "entity.h"

lua_State *script_init(struct server *);
void script_process(lua_State *, struct player *);
void script_onnpctalk(lua_State *, struct player *, struct npc *);
void script_onuseobj(lua_State *, struct player *, const char *);

#endif

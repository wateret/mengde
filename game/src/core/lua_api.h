#ifndef MENGDE_CORE_LUA_API_H_
#define MENGDE_CORE_LUA_API_H_

// C API for Lua binding

extern "C" {

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

#define MACRO_LUA_GAME(cname, luaname) int Game_##cname(lua_State*);

#include "lua_api_game.h.inc"

#undef MACRO_LUA_GAME
}

#endif  // MENGDE_CORE_LUA_API_H_

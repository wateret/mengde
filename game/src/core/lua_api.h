#ifndef MENGDE_CORE_LUA_API_H_
#define MENGDE_CORE_LUA_API_H_

// C API for Lua binding

#define LUA_GAME_TABLE_NAME "Stage"
#define LUA_GAME_OBJ_NAME "game_object"

extern "C" {

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

#define MACRO_LUA_GAME(cname, luaname) int Game_##cname(lua_State*);

#include "lua_api_game.h.inc"

#undef MACRO_LUA_GAME
}

#endif  // MENGDE_CORE_LUA_API_H_

#ifndef LUA_GAME_H_
#define LUA_GAME_H_

// C API for Lua binding

#define LUA_GAME_TABLE_NAME "Game"
#define LUA_GAME_OBJ_NAME   "game_object"

extern "C" {

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define MACRO_LUA_GAME(cname, luaname) \
  int Game_##cname(lua_State*);

#include "lua_game.h.inc"

#undef MACRO_LUA_GAME

}

#endif // LUA_GAME_H_

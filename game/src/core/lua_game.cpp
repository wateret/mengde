#include "lua_game.h"

#include "lua/lua_script.h"
#include "game.h"
#include "cmd.h"

using namespace mengde::core;

static Game* lua_get_game_object(lua_State* L) {
  lua_getglobal(L, LUA_GAME_OBJ_NAME);
  Game* game = static_cast<Game*>(lua_touserdata(L, -1));
  lua_pop(L, 1);
  return game;
}

static Vec2D GetVec2DFromLua(lua::LuaScript* lua) {
  vector<int> vec = lua->GetVector<int>();
  lua->PopStack(1); // FIXME GetVector does not pop a element.
  return {vec[0], vec[1]};
}

#define LUA_IMPL(cname) \
  int Game_##cname(lua_State* L)

LUA_IMPL(AppointHero) {
  Game* game = lua_get_game_object(L);

  lua::LuaScript lua(L);
  uint16_t level = lua.Get<uint16_t>();
  string   id    = lua.Get<string>();

  game->AppointHero(id, level);
  return 0;
}

LUA_IMPL(GenerateOwnUnit) {
  Game* game = lua_get_game_object(L);

  lua::LuaScript lua(L);
  Vec2D  pos = GetVec2DFromLua(&lua);
  string id  = lua.Get<string>();

  int unit_no = game->GenerateOwnUnit(id, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(GenerateUnit) {
  Game* game = lua_get_game_object(L);

  lua::LuaScript lua(L);
  Vec2D      pos   = GetVec2DFromLua(&lua);
  Force force  = (Force)lua.Get<int>();
  uint16_t   level = lua.Get<uint16_t>();
  string     id    = lua.Get<string>();

  int unit_no = game->GenerateUnit(id, level, force, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(ObtainEquipment) {
  Game* game = lua_get_game_object(L);

  lua::LuaScript lua(L);
  uint16_t amount = lua.Get<uint16_t>();
  string   id     = lua.Get<string>();

  game->ObtainEquipment(id, amount);
  return 0;
}

LUA_IMPL(GetNumEnemiesAlive) {
  Game* game = lua_get_game_object(L);
  uint32_t ret = game->GetNumEnemiesAlive();
  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(GetNumOwnsAlive) {
  Game* game = lua_get_game_object(L);
  uint32_t ret = game->GetNumOwnsAlive();
  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(PushCmdMove) {
  Game* game = lua_get_game_object(L);
  lua::LuaScript lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  int unit_id = lua.Get<int>();
  Unit* unit = game->GetUnit(unit_id);
  game->Push(unique_ptr<CmdMove>(new CmdMove(unit, pos)));
  return 0;
}

LUA_IMPL(PushCmdSpeak) {
  Game* game = lua_get_game_object(L);
  lua::LuaScript lua(L);
  string words = lua.Get<string>();
  int unit_id = lua.Get<int>();
  Unit* unit = game->GetUnit(unit_id);
  game->Push(unique_ptr<CmdSpeak>(new CmdSpeak(unit, words)));
  return 0;
}

#undef LUA_IMPL

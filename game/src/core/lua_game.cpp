#include "lua_game.h"

#include "lua/lua_script.h"
#include "game.h"
#include "cmd.h"

using namespace mengde::core;

static Vec2D GetVec2DFromLua(lua::LuaScript* lua) {
  vector<int> vec = lua->GetVector<int>();
  lua->PopStack(1); // FIXME GetVector does not pop a element.
  return {vec[0], vec[1]};
}

#define LUA_IMPL(cname) \
  int Game_##cname(lua_State* L)

LUA_IMPL(AppointHero) {
  lua::LuaScript lua(L);
  uint16_t level = lua.Get<uint16_t>();
  string   id    = lua.Get<string>();
  Game*    game =  lua.Get<Game*>();

  game->AppointHero(id, level);

  return 0;
}

LUA_IMPL(GenerateOwnUnit) {
  lua::LuaScript lua(L);
  Vec2D  pos = GetVec2DFromLua(&lua);
  string id  = lua.Get<string>();
  Game* game = lua.Get<Game*>();

  int unit_no = game->GenerateOwnUnit(id, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(GenerateUnit) {
  lua::LuaScript lua(L);
  Vec2D    pos   = GetVec2DFromLua(&lua);
  Force    force = (Force)lua.Get<int>();
  uint16_t level = lua.Get<uint16_t>();
  string   id    = lua.Get<string>();
  Game*    game  = lua.Get<Game*>();

  int unit_no = game->GenerateUnit(id, level, force, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(ObtainEquipment) {
  lua::LuaScript lua(L);
  uint16_t amount = lua.Get<uint16_t>();
  string   id     = lua.Get<string>();
  Game*    game   = lua.Get<Game*>();

  game->ObtainEquipment(id, amount);

  return 0;
}

LUA_IMPL(GetNumEnemiesAlive) {
  lua::LuaScript lua(L);
  Game* game = lua.Get<Game*>();

  uint32_t ret = game->GetNumEnemiesAlive();

  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(GetNumOwnsAlive) {
  lua::LuaScript lua(L);
  Game* game = lua.Get<Game*>();

  uint32_t ret = game->GetNumOwnsAlive();

  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(PushCmdMove) {
  lua::LuaScript lua(L);
  Vec2D pos     = GetVec2DFromLua(&lua);
  int   unit_id = lua.Get<int>();
  Game* game    = lua.Get<Game*>();

  Unit* unit = game->GetUnit(unit_id);
  game->Push(unique_ptr<CmdMove>(new CmdMove(unit, pos)));

  return 0;
}

LUA_IMPL(PushCmdSpeak) {
  lua::LuaScript lua(L);
  string words = lua.Get<string>();
  int unit_id  = lua.Get<int>();
  Game* game   = lua.Get<Game*>();

  Unit* unit = game->GetUnit(unit_id);
  game->Push(unique_ptr<CmdSpeak>(new CmdSpeak(unit, words)));

  return 0;
}

#undef LUA_IMPL

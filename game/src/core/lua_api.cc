#include "lua_api.h"

#include "cmd.h"
#include "game.h"
#include "lua/lua.h"

using namespace mengde::core;

static Vec2D GetVec2DFromLua(lua::Lua* lua) {
  vector<int> vec = lua->Pop<vector<int>>();
  return {vec[0], vec[1]};
}

#define LUA_IMPL(cname) int Game_##cname(lua_State* L)

LUA_IMPL(AppointHero) {
  lua::Lua lua(L);
  uint16_t level = lua.Pop<uint16_t>();
  string   id    = lua.Pop<string>();
  Game*    game  = lua.Pop<Game*>();

  game->AppointHero(id, level);

  return 0;
}

LUA_IMPL(GenerateOwnUnit) {
  lua::Lua lua(L);
  Vec2D    pos  = GetVec2DFromLua(&lua);
  string   id   = lua.Pop<string>();
  Game*    game = lua.Pop<Game*>();

  int unit_no = game->GenerateOwnUnit(id, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(GenerateUnit) {
  lua::Lua lua(L);
  Vec2D    pos   = GetVec2DFromLua(&lua);
  Force    force = (Force)lua.Pop<int>();
  uint16_t level = lua.Pop<uint16_t>();
  string   id    = lua.Pop<string>();
  Game*    game  = lua.Pop<Game*>();

  int unit_no = game->GenerateUnit(id, level, force, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(ObtainEquipment) {
  lua::Lua lua(L);
  uint16_t amount = lua.Pop<uint16_t>();
  string   id     = lua.Pop<string>();
  Game*    game   = lua.Pop<Game*>();

  game->ObtainEquipment(id, amount);

  return 0;
}

LUA_IMPL(GetNumEnemiesAlive) {
  lua::Lua lua(L);
  Game*    game = lua.Pop<Game*>();

  uint32_t ret = game->GetNumEnemiesAlive();

  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(GetNumOwnsAlive) {
  lua::Lua lua(L);
  Game*    game = lua.Pop<Game*>();

  uint32_t ret = game->GetNumOwnsAlive();

  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(PushCmdMove) {
  lua::Lua lua(L);
  Vec2D    pos     = GetVec2DFromLua(&lua);
  int      unit_id = lua.Pop<int>();
  Game*    game    = lua.Pop<Game*>();

  Unit* unit = game->GetUnit(unit_id);
  game->Push(unique_ptr<CmdMove>(new CmdMove(unit, pos)));

  return 0;
}

LUA_IMPL(PushCmdSpeak) {
  lua::Lua lua(L);
  string   words   = lua.Pop<string>();
  int      unit_id = lua.Pop<int>();
  Game*    game    = lua.Pop<Game*>();

  Unit* unit = game->GetUnit(unit_id);
  game->Push(unique_ptr<CmdSpeak>(new CmdSpeak(unit, words)));

  return 0;
}

#undef LUA_IMPL

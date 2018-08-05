#include "lua_api.h"

#include "cmd.h"
#include "lua/lua.h"
#include "stage.h"

using namespace mengde::core;

static Vec2D GetVec2DFromLua(lua::Lua* lua) {
  vector<int> vec = lua->Pop<vector<int>>();
  return {vec[0], vec[1]};
}

#define LUA_IMPL(cname) int Game_##cname(lua_State* L)

LUA_IMPL(AppointHero) {
  lua::Lua lua(L);
  uint16_t level = lua.Pop<uint16_t>();
  string id = lua.Pop<string>();
  Stage* game = lua.Pop<Stage*>();

  game->AppointHero(id, level);

  return 0;
}

LUA_IMPL(GenerateOwnUnit) {
  lua::Lua lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  string id = lua.Pop<string>();
  Stage* game = lua.Pop<Stage*>();

  int unit_no = game->GenerateOwnUnit(id, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(GenerateUnit) {
  lua::Lua lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  Force force = (Force)lua.Pop<int>();
  uint16_t level = lua.Pop<uint16_t>();
  string id = lua.Pop<string>();
  Stage* game = lua.Pop<Stage*>();

  int unit_no = game->GenerateUnit(id, level, force, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(ObtainEquipment) {
  lua::Lua lua(L);
  uint16_t amount = lua.Pop<uint16_t>();
  string id = lua.Pop<string>();
  Stage* game = lua.Pop<Stage*>();

  game->ObtainEquipment(id, amount);

  return 0;
}

LUA_IMPL(GetNumEnemiesAlive) {
  lua::Lua lua(L);
  Stage* game = lua.Pop<Stage*>();

  uint32_t ret = game->GetNumEnemiesAlive();

  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(GetNumOwnsAlive) {
  lua::Lua lua(L);
  Stage* game = lua.Pop<Stage*>();

  uint32_t ret = game->GetNumOwnsAlive();

  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(PushCmdMove) {
  lua::Lua lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  int unit_id = lua.Pop<int>();
  Stage* game = lua.Pop<Stage*>();

  game->Push(std::make_unique<CmdMove>(unit_id, pos));

  return 0;
}

LUA_IMPL(PushCmdSpeak) {
  lua::Lua lua(L);
  string words = lua.Pop<string>();
  int unit_id = lua.Pop<int>();
  Stage* game = lua.Pop<Stage*>();

  game->Push(std::make_unique<CmdSpeak>(unit_id, words));

  return 0;
}

LUA_IMPL(SetOnDeploy) {
  lua::Lua lua(L);

  auto ref = lua.Pop<lua::Ref>();
  Stage* game = lua.Pop<Stage*>();

  game->SetOnDeploy(ref);

  return 0;
}

LUA_IMPL(SetOnBegin) {
  lua::Lua lua(L);

  auto ref = lua.Pop<lua::Ref>();
  Stage* game = lua.Pop<Stage*>();

  game->SetOnBegin(ref);

  return 0;
}

LUA_IMPL(SetOnVictory) {
  lua::Lua lua(L);

  auto ref = lua.Pop<lua::Ref>();
  Stage* game = lua.Pop<Stage*>();

  game->SetOnVictory(ref);

  return 0;
}

LUA_IMPL(SetOnDefeat) {
  lua::Lua lua(L);

  auto ref = lua.Pop<lua::Ref>();
  Stage* game = lua.Pop<Stage*>();

  game->SetOnDefeat(ref);

  return 0;
}

LUA_IMPL(SetEndCondition) {
  lua::Lua lua(L);

  auto ref = lua.Pop<lua::Ref>();
  Stage* game = lua.Pop<Stage*>();

  game->SetEndCondition(ref);

  return 0;
}

#undef LUA_IMPL

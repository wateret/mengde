#include "lua_api.h"

#include "cmd.h"
#include "luab/lua.h"
#include "stage.h"

using namespace mengde::core;

static Vec2D GetVec2DFromLua(luab::Lua* lua) {
  vector<int> vec = lua->Pop<vector<int>>();
  return {vec[0], vec[1]};
}

#define LUA_IMPL(cname) int Game_##cname(lua_State* L)

LUA_IMPL(AppointHero) {
  luab::Lua lua(L);

  uint16_t level = lua.Pop<uint16_t>();
  string id = lua.Pop<string>();
  Stage* stage = lua.Pop<Stage*>();

  stage->AppointHero(id, level);

  return 0;
}

LUA_IMPL(GenerateOwnUnit) {
  luab::Lua lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  string id = lua.Pop<string>();
  Stage* stage = lua.Pop<Stage*>();

  int unit_no = stage->GenerateOwnUnit(id, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(GenerateUnit) {
  luab::Lua lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  Force force = (Force)lua.Pop<int>();
  uint16_t level = lua.Pop<uint16_t>();
  string id = lua.Pop<string>();
  Stage* stage = lua.Pop<Stage*>();

  int unit_no = stage->GenerateUnit(id, level, force, pos);

  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(ObtainEquipment) {
  luab::Lua lua(L);
  uint16_t amount = lua.Pop<uint16_t>();
  string id = lua.Pop<string>();
  Stage* stage = lua.Pop<Stage*>();

  stage->ObtainEquipment(id, amount);

  return 0;
}

LUA_IMPL(GetNumEnemiesAlive) {
  luab::Lua lua(L);
  Stage* stage = lua.Pop<Stage*>();

  uint32_t ret = stage->GetNumEnemiesAlive();

  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(GetNumOwnsAlive) {
  luab::Lua lua(L);
  Stage* stage = lua.Pop<Stage*>();

  uint32_t ret = stage->GetNumOwnsAlive();

  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(GetUnitInfo) {
  luab::Lua lua(L);

  auto uid = UId{lua.Pop<uint32_t>()};
  Stage* stage = lua.Pop<Stage*>();

  auto unit = stage->LookupUnit(uid);

  // Build a lua table as a return value

  luab::Table table;

  table.Set("id", unit->id());
  table.Set("uid", unit->uid().Value());
  {
    luab::Table pos;
    pos.Set("x", unit->position().x);
    pos.Set("y", unit->position().y);
    table.Set("position", pos);
  }

  lua.PushToStack(table);

  return 1;
}

LUA_IMPL(GetUnitOnPosition) {
  luab::Lua lua{L};
  Vec2D pos = GetVec2DFromLua(&lua);
  Stage* stage = lua.Pop<Stage*>();

  auto unit = stage->GetUnitInCell(pos);
  if (unit) {
    lua.PushToStack(unit->uid().Value());
  } else {
    lua.PushNilToStack();
  }

  return 1;
}

LUA_IMPL(CmdMove) {
  luab::Lua lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  UId uid{lua.Pop<uint32_t>()};
  Stage* stage = lua.Pop<Stage*>();

  stage->Push(std::make_unique<CmdMove>(uid, pos));

  return 0;
}

LUA_IMPL(CmdSpeak) {
  luab::Lua lua(L);
  string words = lua.Pop<string>();
  UId uid{lua.Pop<uint32_t>()};
  Stage* stage = lua.Pop<Stage*>();

  stage->Push(std::make_unique<CmdSpeak>(uid, words));

  return 0;
}

LUA_IMPL(CmdGainExp) {
  luab::Lua lua(L);

  int exp = lua.Pop<int>();
  UId uid{lua.Pop<uint32_t>()};
  Stage* stage = lua.Pop<Stage*>();

  stage->Push(std::make_unique<CmdGainExp>(uid, exp));

  return 0;
}

LUA_IMPL(SetOnDeploy) {
  luab::Lua lua(L);

  auto ref = lua.Pop<luab::Ref>();
  Stage* stage = lua.Pop<Stage*>();

  stage->SetOnDeploy(ref);

  return 0;
}

LUA_IMPL(SetOnBegin) {
  luab::Lua lua(L);

  auto ref = lua.Pop<luab::Ref>();
  Stage* stage = lua.Pop<Stage*>();

  stage->SetOnBegin(ref);

  return 0;
}

LUA_IMPL(SetOnVictory) {
  luab::Lua lua(L);

  auto ref = lua.Pop<luab::Ref>();
  Stage* stage = lua.Pop<Stage*>();

  stage->SetOnVictory(ref);

  return 0;
}

LUA_IMPL(SetOnDefeat) {
  luab::Lua lua(L);

  auto ref = lua.Pop<luab::Ref>();
  Stage* stage = lua.Pop<Stage*>();

  stage->SetOnDefeat(ref);

  return 0;
}

LUA_IMPL(SetEndCondition) {
  luab::Lua lua(L);

  auto ref = lua.Pop<luab::Ref>();
  Stage* stage = lua.Pop<Stage*>();

  stage->SetEndCondition(ref);

  return 0;
}

LUA_IMPL(RegisterEvent) {
  luab::Lua lua(L);

  auto handler = lua.Pop<luab::Ref>();
  auto condition = lua.Pop<luab::Ref>();
  Stage* stage = lua.Pop<Stage*>();

  auto id = stage->RegisterEvent(condition, handler);

  lua.PushToStack(id);

  return 1;
}

LUA_IMPL(UnregisterEvent) {
  luab::Lua lua(L);

  auto id = lua.Pop<uint32_t>();
  Stage* stage = lua.Pop<Stage*>();

  stage->UnregisterEvent(id);

  return 0;
}

LUA_IMPL(SetAIMode) {
  luab::Lua lua{L};
  auto ai_mode = StringToAIMode(lua.Pop<string>());
  auto uid = mengde::core::UId{lua.Pop<uint32_t>()};
  auto stage = lua.Pop<Stage*>();

  if (ai_mode == mengde::core::AIMode::kNone) {
    // TODO Handle error in a better way
    throw "Unknown AI Mode";
  }

  stage->SetAIMode(uid, ai_mode);

  return 0;
}

#undef LUA_IMPL

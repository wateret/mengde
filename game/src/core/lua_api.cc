#include "lua_api.h"

#include "cell.h"
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
  table.Set("level", unit->GetLevel());
  table.Set("hero_class", unit->unit_class()->id());
  {
    luab::Table pos;
    pos.Set("x", unit->position().x);
    pos.Set("y", unit->position().y);
    table.Set("position", pos);
  }

  auto build_attr_table = [](const Attribute& obj) {
    luab::Table attr_table;
    attr_table.Set("atk", obj.atk);
    attr_table.Set("def", obj.def);
    attr_table.Set("dex", obj.dex);
    attr_table.Set("itl", obj.itl);
    attr_table.Set("mor", obj.mor);
    return attr_table;
  };

  table.Set("hero_attr", build_attr_table(unit->hero().GetHeroStatBase()));

  {
    luab::Table unit_attr;
    unit_attr.Set("base", build_attr_table(unit->GetOriginalAttr()));
    unit_attr.Set("current", build_attr_table(unit->GetCurrentAttr()));
    table.Set("unit_attr", unit_attr);
  }

  {
    table.Set("cur_hp", unit->GetCurrentHpMp().hp);
    table.Set("cur_mp", unit->GetCurrentHpMp().mp);
    table.Set("max_hp", unit->GetOriginalHpMp().hp);
    table.Set("max_mp", unit->GetOriginalHpMp().mp);
  }

  {
    luab::Table attack_range;
    uint32_t idx = 0;
    unit->attack_range().ForEach([&](const Vec2D& pos) {
      luab::Table element;
      element.Set("x", pos.x);
      element.Set("y", pos.y);
      attack_range.Set(std::to_string(idx), element);  // TODO support array table(key with int(or any) value type)
      idx++;
    });
    table.Set("attack_range", attack_range);
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

LUA_IMPL(GetTerrainOnPosition) {
  luab::Lua lua{L};
  Vec2D pos = GetVec2DFromLua(&lua);
  Stage* stage = lua.Pop<Stage*>();

  lua.PushToStack(stage->GetCell(pos)->GetTerrainId());

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

LUA_IMPL(CmdKill) {
  luab::Lua lua(L);

  UId uid{lua.Pop<uint32_t>()};
  Stage* stage = lua.Pop<Stage*>();

  stage->Push(std::make_unique<CmdKilled>(uid));

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

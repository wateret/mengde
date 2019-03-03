#include "lua_game.h"

#include <memory>

#include "cell.h"
#include "cmds.h"

namespace mengde {
namespace core {

Vec2D LuaGame::TableToVec2D(const sol::table& table) {
  Vec2D vec2d{table[1], table[2]};
  return vec2d;
}

sol::table LuaGame::Vec2DToTable(Vec2D vec, sol::state_view& lua) {
  auto table = lua.create_table();
  table[1] = vec.x;
  table[2] = vec.y;
  return table;
}

LuaGame::LuaGame(Stage* stage) : stage_{stage} {}

void LuaGame::AppointHero(const string& id, uint16_t level) { stage_->AppointHero(id, level); }

uint32_t LuaGame::GenerateOwnUnit(const string& id, const sol::table& pos) {
  return stage_->GenerateOwnUnit(id, TableToVec2D(pos));
}

uint32_t LuaGame::GenerateUnit(const string& id, uint16_t level, Force force, const sol::table& pos) {
  return stage_->GenerateUnit(id, level, force, TableToVec2D(pos));
}

void LuaGame::ObtainEquipment(const string& id, uint32_t amount) { stage_->ObtainEquipment(id, amount); }

uint32_t LuaGame::GetNumEnemiesAlive() { return stage_->GetNumEnemiesAlive(); }

uint32_t LuaGame::GetNumOwnsAlive() { return stage_->GetNumOwnsAlive(); }

sol::table LuaGame::GetUnitInfo(uint32_t uid, sol::this_state ts) {
  sol::state_view lua{ts};

  auto table = lua.create_table();

  auto unit = stage_->LookupUnit(UId{uid});

  table["id"] = unit->id();
  table["uid"] = unit->uid().Value();
  table["level"] = unit->GetLevel();
  table["hero_class"] = unit->unit_class()->id();
  table["position"] = Vec2DToTable(unit->position(), lua);

  auto build_attr_table = [&](const Attribute& obj) {
    auto attr_table = lua.create_table();
    attr_table["atk"] = obj.atk;
    attr_table["def"] = obj.def;
    attr_table["dex"] = obj.dex;
    attr_table["itl"] = obj.itl;
    attr_table["mor"] = obj.mor;
    return attr_table;
  };

  table["hero_attr"] = build_attr_table(unit->hero().GetHeroStatBase());

  {
    auto unit_attr = lua.create_table();
    unit_attr["base"] = build_attr_table(unit->GetOriginalAttr());
    unit_attr["current"] = build_attr_table(unit->GetCurrentAttr());
    table["unit_attr"] = unit_attr;
  }

  {
    table["cur_hp"] = unit->GetCurrentHpMp().hp;
    table["cur_mp"] = unit->GetCurrentHpMp().mp;
    table["max_hp"] = unit->GetOriginalHpMp().hp;
    table["max_mp"] = unit->GetOriginalHpMp().mp;
  }

  {
    auto attack_range = lua.create_table();
    uint32_t idx = 1;
    unit->attack_range().ForEach([&](const Vec2D& pos) {
      attack_range[idx] = Vec2DToTable(pos, lua);
      idx++;
    });
    table["attack_range"] = attack_range;
  }
  return table;
}

sol::object LuaGame::GetUnitOnPosition(const sol::table& pos, sol::this_state ts) {
  auto unit = stage_->GetUnitInCell(TableToVec2D(pos));
  if (unit) {
    return sol::make_object(ts, unit->uid().Value());
  } else {
    return sol::nil;
  }
}

sol::object LuaGame::GetTerrainOnPosition(const sol::table& pos, sol::this_state ts) {
  auto cell = stage_->GetCell(TableToVec2D(pos));
  if (cell) {
    return sol::make_object(ts, cell->GetTerrainId());
  } else {
    return sol::nil;
  }
}

void LuaGame::SetOnDeploy(const sol::function& fn) { stage_->SetOnDeploy(fn); }

void LuaGame::SetOnBegin(const sol::function& fn) { stage_->SetOnBegin(fn); }

void LuaGame::SetOnVictory(const sol::function& fn) { stage_->SetOnVictory(fn); }

void LuaGame::SetOnDefeat(const sol::function& fn) { stage_->SetOnDefeat(fn); }

void LuaGame::SetEndCondition(const sol::function& fn) { stage_->SetEndCondition(fn); }

uint32_t LuaGame::RegisterEvent(const sol::function& predicate, const sol::function& handler) {
  return stage_->RegisterEvent(predicate, handler);
}

void LuaGame::UnregisterEvent(uint32_t id) { stage_->UnregisterEvent(id); }

void LuaGame::SetAIMode(uint32_t uid, const string& ai_mode_str) {
  auto ai_mode = StringToAIMode(ai_mode_str);
  stage_->SetAIMode(UId{uid}, ai_mode);
}

void LuaGame::PushCmdMove(uint32_t uid, const sol::table& pos) {
  stage_->Push(std::make_unique<CmdMove>(UId{uid}, TableToVec2D(pos)));
}

void LuaGame::PushCmdSpeak(uint32_t uid, const string& words) {
  stage_->Push(std::make_unique<CmdSpeak>(UId{uid}, words));
}

void LuaGame::PushCmdGainExp(uint32_t uid, int32_t amount) {
  stage_->Push(std::make_unique<CmdGainExp>(UId{uid}, amount));
}

void LuaGame::PushCmdKill(uint32_t uid) { stage_->Push(std::make_unique<CmdKilled>(UId{uid})); }

}  // namespace core
}  // namespace mengde

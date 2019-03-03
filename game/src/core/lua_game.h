#ifndef MENGDE_CORE_LUA_GAME_H_
#define MENGDE_CORE_LUA_GAME_H_

#include <sol.hpp>

#include "stage.h"

namespace mengde {
namespace core {

class Stage;

// Lua API class
// Used as sol usertype
class LuaGame {
 private:
  Vec2D TableToVec2D(const sol::table& table);
  sol::table Vec2DToTable(Vec2D vec, sol::state_view& lua);

 public:
  LuaGame(Stage* stage);

 public:
  void AppointHero(const string& id, uint16_t level);
  uint32_t GenerateOwnUnit(const string& id, const sol::table& pos);
  uint32_t GenerateUnit(const string& id, uint16_t level, Force force, const sol::table& pos);
  void ObtainEquipment(const string& id, uint32_t amount);
  void SetAIMode(uint32_t uid, const string& ai_mode_str);

  uint32_t GetNumEnemiesAlive();
  uint32_t GetNumOwnsAlive();
  sol::table GetUnitInfo(uint32_t uid, sol::this_state ts);
  sol::object GetUnitOnPosition(const sol::table& pos, sol::this_state ts);
  sol::object GetTerrainOnPosition(const sol::table& pos, sol::this_state ts);

  void SetOnDeploy(const sol::function& fn);
  void SetOnBegin(const sol::function& fn);
  void SetOnVictory(const sol::function& fn);
  void SetOnDefeat(const sol::function& fn);
  void SetEndCondition(const sol::function& fn);
  uint32_t RegisterEvent(const sol::function& condition, const sol::function& handler);
  void UnregisterEvent(uint32_t id);

  void PushCmdMove(uint32_t uid, const sol::table& pos);
  void PushCmdSpeak(uint32_t uid, const string& words);
  void PushCmdGainExp(uint32_t uid, int32_t amount);
  void PushCmdKill(uint32_t uid);

 private:
  Stage* stage_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_LUA_GAME_H_

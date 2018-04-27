#ifndef MENGDE_CORE_USER_INTERFACE_H_
#define MENGDE_CORE_USER_INTERFACE_H_

#include <stdint.h>

#include "cmd.h"
#include "util/common.h"

namespace mengde {
namespace core {

class Game;
class Scenario;
class Unit;

class UserInterface {
 public:
  UserInterface(Game* stage);

 public:
  vector<Vec2D> QueryMovablePositions(uint32_t unit_id);
  vector<unique_ptr<CmdAct>> QueryPossibleActs(uint32_t unit_id, uint32_t pos_id, ActionType type);
  void PushAction(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id);

 private:
  Unit* GetUnit(uint32_t unit_id);
  Vec2D GetMovedPosition(uint32_t unit_id, uint32_t move_id);
  unique_ptr<CmdAct> GetActCmd(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id);

 private:
  Game* stage_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_USER_INTERFACE_H_

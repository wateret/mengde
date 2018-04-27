#include "user_interface.h"

#include "game.h"
#include "unit.h"
#include "cmd.h"

namespace mengde {
namespace core {

UserInterface::UserInterface(Game* stage) : stage_(stage) {}

Unit* UserInterface::GetUnit(uint32_t unit_id) { return stage_->GetUnit(unit_id); }

vector<Vec2D> UserInterface::QueryMovablePositions(uint32_t unit_id) {
  Unit* unit = GetUnit(unit_id);
  return stage_->FindMovablePos(unit);
}

vector<unique_ptr<CmdAct>> UserInterface::QueryPossibleActs(uint32_t unit_id, uint32_t pos_id, ActionType type) {
  Unit* unit = GetUnit(unit_id);
  Vec2D move_pos = GetMovedPosition(unit_id, pos_id);
  UNUSED(move_pos); // Later we may need move_pos
  vector<unique_ptr<CmdAct>> list;
  switch (type) {
    case ActionType::kStay:
      list.push_back(unique_ptr<CmdStay>(new CmdStay(unit)));
      break;

    case ActionType::kBasicAttack:
      UNREACHABLE("NYI for ActionType of kBasicAttack");
      break;

    case ActionType::kMagic:
      UNREACHABLE("NYI for ActionType of kMagic");
      break;

    default:
      UNREACHABLE("Invalid ActionType");
      break;
  };
  return list;
}

void UserInterface::PushAction(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id) {
  Unit* unit             = GetUnit(unit_id);
  Vec2D move_pos         = GetMovedPosition(unit_id, move_id);
  unique_ptr<CmdAct> act = GetActCmd(unit_id, move_id, type, act_id);

  CmdAction* cmd = new CmdAction();
  cmd->SetCmdMove(unique_ptr<CmdMove>(new CmdMove(unit, move_pos)));
  cmd->SetCmdAct(std::move(act));

  stage_->Push(unique_ptr<CmdAction>(cmd));
}

Vec2D UserInterface::GetMovedPosition(uint32_t unit_id, uint32_t move_id) {
  auto moves = QueryMovablePositions(unit_id);
  ASSERT(unit_id < moves.size());
  return moves[move_id];
}

unique_ptr<CmdAct> UserInterface::GetActCmd(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id) {
  auto acts = QueryPossibleActs(unit_id, move_id, type);
  ASSERT(act_id < acts.size());
  return std::move(acts[act_id]);
}

}  // namespace core
}  // namespace mengde

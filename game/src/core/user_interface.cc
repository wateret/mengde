#include "user_interface.h"

#include "cmd.h"
#include "game.h"
#include "unit.h"

namespace mengde {
namespace core {

// AvailableMoves

AvailableMoves::AvailableMoves(Game* stage, Unit* unit) { moves_ = stage->FindMovablePos(unit); }

Vec2D AvailableMoves::Get(uint32_t idx) {
  ASSERT(idx < moves_.size());
  return moves_[idx];
}

// AvailableActs

AvailableActs::AvailableActs(Game* stage, Unit* unit, Vec2D move_pos, ActionType type) {
  switch (type) {
    case ActionType::kStay:
      acts_.push_back(unique_ptr<CmdStay>(new CmdStay(unit)));
      break;

    case ActionType::kBasicAttack: {
      Unit* atk = unit;
      unit->GetAttackRange().ForEach(
          [&](Vec2D pos) {
            if (!stage->IsValidCoords(pos)) return;
            Unit* def = stage->GetUnitInCell(pos);
            if (def != nullptr && atk->IsHostile(def)) {
              acts_.push_back(unique_ptr<core::CmdBasicAttack>(
                  new core::CmdBasicAttack(atk, def, core::CmdBasicAttack::Type::kActive)));
            }
          },
          move_pos);
      break;
    }

    case ActionType::kMagic:
      UNREACHABLE("NYI for ActionType of kMagic");
      break;

    default:
      UNREACHABLE("Invalid ActionType");
      break;
  };

  ASSERT(type != ActionType::kStay || acts_.size() == 1);
}

unique_ptr<CmdAct> AvailableActs::Get(uint32_t idx) {
  ASSERT(idx < acts_.size());
  return std::move(acts_[idx]);
}

// UserInterface

UserInterface::UserInterface(Game* stage) : stage_(stage) {}

Unit* UserInterface::GetUnit(uint32_t unit_id) { return stage_->GetUnit(unit_id); }

AvailableMoves UserInterface::QueryMoves(uint32_t unit_id) {
  Unit* unit = GetUnit(unit_id);
  return AvailableMoves(stage_, unit);
}

AvailableActs UserInterface::QueryActs(uint32_t unit_id, uint32_t move_id, ActionType type) {
  Unit* unit     = GetUnit(unit_id);
  Vec2D move_pos = GetMovedPosition(unit_id, move_id);
  return AvailableActs(stage_, unit, move_pos, type);
}

void UserInterface::PushAction(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id) {
  Unit*              unit     = GetUnit(unit_id);
  Vec2D              move_pos = GetMovedPosition(unit_id, move_id);
  unique_ptr<CmdAct> act      = GetActCmd(unit_id, move_id, type, act_id);

  CmdAction* cmd = new CmdAction();
  cmd->SetCmdMove(unique_ptr<CmdMove>(new CmdMove(unit, move_pos)));
  cmd->SetCmdAct(std::move(act));

  stage_->Push(unique_ptr<CmdAction>(cmd));
}

Vec2D UserInterface::GetMovedPosition(uint32_t unit_id, uint32_t move_id) {
  auto moves = QueryMoves(unit_id);
  return moves.Get(move_id);
}

unique_ptr<CmdAct> UserInterface::GetActCmd(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id) {
  auto acts = QueryActs(unit_id, move_id, type);
  return acts.Get(act_id);
}

}  // namespace core
}  // namespace mengde

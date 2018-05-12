#include "user_interface.h"

#include "cmd.h"
#include "game.h"
#include "path_tree.h"
#include "unit.h"

namespace mengde {
namespace core {

// AvailableUnits

AvailableUnits::AvailableUnits(Game* stage) {
  stage->ForEachUnitIdxConst([&](uint32_t idx, const Unit* unit) {
    if (unit->GetForce() == stage->GetCurrentForce() && !unit->IsDoneAction()) {
      LOG_DEBUG("AVAILABLEUNITS %d %d", unit->GetPosition().x, unit->GetPosition().y);
      units_.push_back(std::make_pair(idx, unit->GetPosition()));
    }
  });
}

uint32_t AvailableUnits::Get(uint32_t idx) {
  ASSERT(idx < units_.size());
  return units_[idx].first;
}

boost::optional<uint32_t> AvailableUnits::FindByPos(Vec2D pos) {
  for (auto& e : units_) {
    if (pos == e.second) {
      return e.first;
    }
  }
  return boost::none;
}

// AvailableMoves

AvailableMoves::AvailableMoves(Game* stage, uint32_t unit_id) {
  Unit* unit = stage->GetUnit(unit_id);
  moves_     = stage->FindMovablePos(unit);
}

Vec2D AvailableMoves::Get(uint32_t idx) {
  ASSERT(idx < moves_.size());
  return moves_[idx];
}

// AvailableActs

AvailableActs::AvailableActs(Game* stage, uint32_t unit_id, uint32_t move_id, ActionType type) : type_(type) {
  Unit* unit     = stage->GetUnit(unit_id);
  Vec2D move_pos = AvailableMoves(stage, unit_id).Get(move_id);

  switch (type) {
    case ActionType::kStay:
      acts_.push_back(unique_ptr<CmdStay>(new CmdStay(unit)));
      break;

    case ActionType::kBasicAttack: {
      Unit* atk = unit;
      atk->GetAttackRange().ForEach(
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

uint32_t AvailableActs::Find(Vec2D pos) {
  uint32_t idx = 0;
  for (auto&& act : acts_) {
    ASSERT(act != nullptr);
    Unit* def = act->GetUnitDef();
    if (def->GetPosition() == pos) {
      return idx;
    }
    idx++;
  }
  return 0;
}

// UserInterface

UserInterface::UserInterface(Game* stage) : stage_(stage) {}

AvailableUnits UserInterface::QueryUnits() { return AvailableUnits(stage_); }

Unit* UserInterface::GetUnit(uint32_t unit_id) { return stage_->GetUnit(unit_id); }

const Unit* UserInterface::GetUnit(Vec2D pos) const {
  Map* map = stage_->GetMap();
  if (map->UnitInCell(pos)) {
    return map->GetUnit(pos);
  } else {
    return nullptr;
  }
}

const Unit* UserInterface::GetUnit(uint32_t unit_id) const { return stage_->GetUnit(unit_id); }

vector<Vec2D> UserInterface::GetPath(uint32_t unit_id, Vec2D pos) const {
  Map* map = stage_->GetMap();
  return map->FindPathTo(GetUnit(unit_id), pos);
}

AvailableMoves UserInterface::QueryMoves(uint32_t unit_id) { return AvailableMoves(stage_, unit_id); }

AvailableActs UserInterface::QueryActs(uint32_t unit_id, uint32_t move_id, ActionType type) {
  return AvailableActs(stage_, unit_id, move_id, type);
}

void UserInterface::PushAction(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id) {
  Unit*              unit = GetUnit(unit_id);
  Vec2D              pos  = GetMovedPosition(unit_id, move_id);
  unique_ptr<CmdAct> act  = GetActCmd(unit_id, move_id, type, act_id);

  CmdAction* cmd = new CmdAction(stage_->IsUserTurn() ? CmdAction::Flag::kUserInput : CmdAction::Flag::kDecompose);
  cmd->SetCmdMove(unique_ptr<CmdMove>(new CmdMove(unit, pos)));
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

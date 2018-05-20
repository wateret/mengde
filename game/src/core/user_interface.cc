#include "user_interface.h"

#include "cell.h"
#include "cmd.h"
#include "game.h"
#include "magic_list.h"
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
  moves_ = stage->FindMovablePos(unit);
}

Vec2D AvailableMoves::Get(uint32_t idx) {
  ASSERT(idx < moves_.size());
  return moves_[idx];
}

void AvailableMoves::ForEach(const std::function<void(uint32_t, Vec2D)>& fn) {
  for (uint32_t i = 0, size = moves_.size(); i < size; i++) {
    fn(i, moves_[i]);
  }
}

// AvailableActs

AvailableActs::AvailableActs(Game* stage, uint32_t unit_id, uint32_t move_id, ActionType type) : type_(type) {
  Unit* unit = stage->GetUnit(unit_id);
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
              acts_.push_back(unique_ptr<CmdBasicAttack>(new CmdBasicAttack(atk, def, CmdBasicAttack::Type::kActive)));
            }
          },
          move_pos);
      break;
    }

    case ActionType::kMagic: {
      Unit* atk = unit;
      MagicList magic_list(stage->GetMagicManager(), unit);

      for (int i = 0; i < magic_list.NumMagics(); i++) {
        Magic* magic = magic_list.GetMagic(i);
        magic->GetRange().ForEach(
            [&](Vec2D pos) {
              if (!stage->IsValidCoords(pos)) return;
              Unit* def = stage->GetUnitInCell(pos);
              if (def != nullptr && atk->IsHostile(def) == magic->GetIsTargetEnemy()) {
                acts_.push_back(unique_ptr<CmdMagic>(new CmdMagic(atk, def, magic)));
              }
            },
            move_pos);
      }
      break;
    }

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

boost::optional<uint32_t> AvailableActs::Find(Vec2D pos) {
  ASSERT(type_ == ActionType::kBasicAttack);

  uint32_t idx = 0;
  for (auto&& act : acts_) {
    Unit* def = act->GetUnitDef();
    if (def->GetPosition() == pos) {
      return idx;
    }
    idx++;
  }
  return boost::none;
}

boost::optional<uint32_t> AvailableActs::FindMagic(const string& magic_id, Vec2D pos) {
  ASSERT(type_ == ActionType::kMagic);

  uint32_t idx = 0;
  for (auto&& act : acts_) {
    CmdMagic* cm = dynamic_cast<CmdMagic*>(act.get());
    ASSERT(cm != nullptr);
    const Magic* magic = cm->magic();
    Unit* def = cm->GetUnitDef();
    if (magic->GetId() == magic_id && def->GetPosition() == pos) {
      return idx;
    }
    idx++;
  }
  return boost::none;
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

const Cell* UserInterface::GetCell(Vec2D pos) const { return stage_->GetCell(pos); }

vector<Vec2D> UserInterface::GetPath(uint32_t unit_id, Vec2D pos) const {
  Map* map = stage_->GetMap();
  return map->FindPathTo(GetUnit(unit_id), pos);
}

Vec2D UserInterface::GetMapSize() const { return stage_->GetMapSize(); }

string UserInterface::GetMapId() const { return stage_->GetMapBitmapPath(); }

bool UserInterface::HasNextCmd() const { return stage_->HasNext(); }

AvailableMoves UserInterface::QueryMoves(uint32_t unit_id) { return AvailableMoves(stage_, unit_id); }

AvailableActs UserInterface::QueryActs(uint32_t unit_id, uint32_t move_id, ActionType type) {
  return AvailableActs(stage_, unit_id, move_id, type);
}

void UserInterface::PushAction(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id) {
  Unit* unit = GetUnit(unit_id);
  Vec2D pos = GetMovedPosition(unit_id, move_id);
  unique_ptr<CmdAct> act = GetActCmd(unit_id, move_id, type, act_id);

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

void UserInterface::ForEachUnit(const std::function<void(uint32_t, const Unit*)>& fn) const {
  stage_->ForEachUnitIdxConst(fn);
}

}  // namespace core
}  // namespace mengde

#include "user_interface.h"

#include "cell.h"
#include "cmd.h"
#include "magic_list.h"
#include "path_tree.h"
#include "stage.h"
#include "turn.h"
#include "unit.h"

namespace mengde {
namespace core {

//
// AvailableUnits
//

AvailableUnits::AvailableUnits(Stage* stage) {
  stage->ForEachUnitConst([&](const Unit* unit) {
    if (unit->force() == stage->GetCurrentForce() && !unit->IsDoneAction()) {
      units_.push_back(std::make_pair(unit->uid(), unit->position()));
    }
  });
}

UId AvailableUnits::Get(const UnitKey& ukey) {
  ASSERT(ukey);
  ASSERT_LE(ukey.Value(), units_.size());
  return units_[ukey.Value()].first;
}

UnitKey AvailableUnits::FindByPos(Vec2D pos) {
  uint32_t idx = 0;
  for (auto& e : units_) {
    if (pos == e.second) {
      return UnitKey{idx};
    }
    idx++;
  }
  return UnitKey{};
}

//
// AvailableMoves
//

AvailableMoves::AvailableMoves(Stage* stage, const UnitKey& ukey) {
  auto uid = AvailableUnits(stage).Get(ukey);
  Unit* unit = stage->LookupUnit(uid);

  if (unit->condition_set().Has(Condition::kStunned) || unit->condition_set().Has(Condition::kRooted)) {
    moves_.push_back(unit->position());
  } else {
    moves_ = stage->FindMovablePos(unit);
  }
}

Vec2D AvailableMoves::Get(const MoveKey& mkey) {
  ASSERT(mkey);
  ASSERT_LE(mkey.Value(), moves_.size());
  return moves_[mkey.Value()];
}

void AvailableMoves::ForEach(const std::function<void(const MoveKey&, Vec2D)>& fn) {
  for (uint32_t i = 0, size = moves_.size(); i < size; i++) {
    fn(MoveKey{i}, moves_[i]);
  }
}

//
// AvailableActs
//

AvailableActs::AvailableActs(Stage* stage, const UnitKey& ukey, const MoveKey& move_id, ActionType type)
    : stage_(stage), type_(type) {
  UId uid = AvailableUnits(stage).Get(ukey);
  Vec2D move_pos = AvailableMoves(stage, ukey).Get(move_id);

  switch (type) {
    case ActionType::kStay:
      acts_.push_back(std::make_unique<CmdStay>(uid));
      break;

    case ActionType::kBasicAttack: {
      auto atk_id = uid;
      Unit* atk = stage_->LookupUnit(uid);

      if (!atk->condition_set().Has(Condition::kStunned)) {
        atk->attack_range().ForEach(
            [&](Vec2D pos) {
              if (!stage->IsValidCoords(pos)) return;
              auto def = stage->GetUnitInCell(pos);
              if (def != nullptr && atk->IsHostile(def)) {
                acts_.push_back(std::make_unique<CmdBasicAttack>(atk_id, def->uid(), CmdBasicAttack::Type::kActive));
              }
            },
            move_pos);
      }
      break;
    }

    case ActionType::kMagic: {
      auto atk_id = uid;
      Unit* atk = stage_->LookupUnit(uid);

      if (!atk->condition_set().Has(Condition::kStunned)) {
        MagicList magic_list(stage->magic_manager(), atk);

        for (int i = 0; i < magic_list.NumMagics(); i++) {
          Magic* magic = magic_list.GetMagic(i);
          magic->GetRange().ForEach(
              [&](Vec2D pos) {
                if (!stage->IsValidCoords(pos)) return;
                const Unit* def = stage->GetUnitInCell(pos);
                if (def != nullptr && atk->IsHostile(def) == magic->is_target_enemy()) {
                  acts_.push_back(std::make_unique<CmdMagic>(atk_id, def->uid(), magic));
                }
              },
              move_pos);
        }
      }
      break;
    }

    default:
      UNREACHABLE("Invalid ActionType");
      break;
  };

  ASSERT(type != ActionType::kStay || acts_.size() == 1);
}

unique_ptr<CmdAct> AvailableActs::Get(const ActKey& akey) {
  ASSERT(akey);
  ASSERT_LE(akey.Value(), acts_.size());
  return std::move(acts_[akey.Value()]);
}

ActKey AvailableActs::Find(Vec2D pos) {
  ASSERT(type_ == ActionType::kBasicAttack);

  uint32_t idx = 0;
  for (auto&& act : acts_) {
    // TODO Keep position info in the object rather than querying stage here
    Unit* def = stage_->LookupUnit(act->GetUnitDef());
    if (def->position() == pos) {
      return ActKey{idx};
    }
    idx++;
  }
  return ActKey{};
}

ActKey AvailableActs::FindMagic(const string& magic_id, Vec2D pos) {
  ASSERT(type_ == ActionType::kMagic);

  uint32_t idx = 0;
  for (auto&& act : acts_) {
    CmdMagic* cm = dynamic_cast<CmdMagic*>(act.get());
    ASSERT(cm != nullptr);
    const Magic* magic = cm->magic();
    // TODO Keep position info in the object rather than querying stage here
    Unit* def = stage_->LookupUnit(cm->GetUnitDef());
    if (magic->GetId() == magic_id && def->position() == pos) {
      return ActKey{idx};
    }
    idx++;
  }
  return ActKey{};
}

// UserInterface

UserInterface::UserInterface(Stage* stage) : stage_(stage) {}

AvailableUnits UserInterface::QueryUnits() const { return AvailableUnits(stage_); }

const Unit* UserInterface::GetUnit(Vec2D pos) const { return stage_->GetUnitInCell(pos); }

const Unit* UserInterface::GetUnit(const UId& unit_id) const { return stage_->LookupUnit(unit_id); }

const Unit* UserInterface::GetUnit(const UnitKey& unit_key) const { return GetUnit(QueryUnits().Get(unit_key)); }

const Cell* UserInterface::GetCell(Vec2D pos) const { return stage_->GetCell(pos); }

vector<Vec2D> UserInterface::GetPath(const UId& unit_id, Vec2D pos) const {
  Map* map = stage_->GetMap();
  return map->FindPathTo(unit_id, pos);
}

const IAIUnit* UserInterface::GetAIUnit(const UnitKey& unit_key) const {
  auto uid = AvailableUnits(stage_).Get(unit_key);
  return stage_->GetAIUnit(uid);
}

Vec2D UserInterface::GetMapSize() const { return stage_->GetMapSize(); }

string UserInterface::GetMapId() const { return stage_->GetMapId(); }

const Map* UserInterface::GetMap() const { return stage_->GetMap(); }

bool UserInterface::HasNextCmd() const { return stage_->HasNext(); }

const Cmd* UserInterface::GetNextCmd() const { return stage_->GetNextCmdConst(); }

void UserInterface::DoNextCmd() { stage_->DoNext(); }

AvailableMoves UserInterface::QueryMoves(const UnitKey& unit_key) const { return AvailableMoves(stage_, unit_key); }

AvailableActs UserInterface::QueryActs(const UnitKey& unit_key, const MoveKey& move_id, ActionType type) const {
  return AvailableActs(stage_, unit_key, move_id, type);
}

void UserInterface::PushAction(const UnitKey& unit_key, const MoveKey& move_id, ActionType type, const ActKey& act_id) {
  UId uid = QueryUnits().Get(unit_key);
  Vec2D pos = GetMovedPosition(unit_key, move_id);
  unique_ptr<CmdAct> act = GetActCmd(unit_key, move_id, type, act_id);

  CmdAction* cmd = new CmdAction(stage_->IsUserTurn() ? CmdAction::Flag::kUserInput : CmdAction::Flag::kDecompose);
  cmd->SetCmdMove(std::make_unique<CmdMove>(uid, pos));
  cmd->SetCmdAct(std::move(act));

  stage_->Push(unique_ptr<CmdAction>(cmd));
}

void UserInterface::PushPlayAI() { stage_->Push(std::make_unique<core::CmdPlayAI>()); }

Vec2D UserInterface::GetMovedPosition(const UnitKey& unit_key, const MoveKey& move_id) {
  auto moves = QueryMoves(unit_key);
  return moves.Get(move_id);
}

unique_ptr<CmdAct> UserInterface::GetActCmd(const UnitKey& unit_key, const MoveKey& move_id, ActionType type,
                                            const ActKey& act_id) {
  auto acts = QueryActs(unit_key, move_id, type);
  return acts.Get(act_id);
}

void UserInterface::ForEachUnit(const std::function<void(const Unit*)>& fn) const { stage_->ForEachUnit(fn); }

bool UserInterface::IsUserTurn() const { return stage_->IsUserTurn(); }

bool UserInterface::IsAITurn() const { return stage_->IsAITurn(); }

const Turn& UserInterface::GetTurn() const { return stage_->GetTurn(); }

bool UserInterface::IsValidCoords(Vec2D c) const { return stage_->IsValidCoords(c); }

std::shared_ptr<core::MagicList> UserInterface::GetMagicList(const UId& uid) const {
  auto unit = GetUnit(uid);
  return std::make_shared<core::MagicList>(stage_->magic_manager(), unit);
}

const Magic* UserInterface::GetMagic(const string& id) const { return stage_->LookupMagic(id); }

const HeroClass* UserInterface::GetUnitClass(const string& id) const {
  auto ucm = stage_->unit_class_manager();
  return ucm->Get(id);
}

}  // namespace core
}  // namespace mengde

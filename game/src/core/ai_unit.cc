#include "ai_unit.h"

#include "user_interface.h"

namespace mengde {
namespace core {

// AIUnitNone

void AIUnitNone::play(const UnitKey&, UserInterface*) const { throw std::runtime_error("Unreachable"); }

// AIUnitRandom

void AIUnitRandom::play(const UnitKey& ukey, UserInterface* ui) const {
  AvailableMoves moves = ui->QueryMoves(ukey);

  MoveKey mkey = GenRandom(moves.Count());

  AvailableActs acts = ui->QueryActs(ukey, mkey, ActionType::kBasicAttack);
  if (acts.Count() > 0) {
    ui->PushAction(ukey, mkey, ActionType::kBasicAttack, 0 /* Simply choose first one */);
  } else {
    ui->PushAction(ukey, GenRandom(moves.Count()), ActionType::kStay, 0);
  }
}

// AIUnitDoNothing

void AIUnitDoNothing::play(const UnitKey& ukey, UserInterface* ui) const {
  ui->PushAction(ukey, MoveKey{0u}, ActionType::kStay, 0);
}

// AIUnitUnitInRange

void AIUnitUnitInRangeRandom::play(const UnitKey& ukey, UserInterface* ui) const {
  AvailableMoves moves = ui->QueryMoves(ukey);

  bool found_target = false;
  MoveKey mkey = 0;
  moves.ForEach([&](const MoveKey& mk, Vec2D) {
    AvailableActs acts = ui->QueryActs(ukey, mk, ActionType::kBasicAttack);
    if (acts.Count() > 0 && !found_target) {
      found_target = true;
      mkey = mk;
    }
  });

  if (found_target) {
    ui->PushAction(ukey, mkey, ActionType::kBasicAttack, 0 /* Simply choose first one */);
  } else {
    ui->PushAction(ukey, GenRandom(moves.Count()), ActionType::kStay, 0);
  }
}

// AIUnitHoldPosition

void AIUnitHoldPosition::play(const UnitKey& ukey, UserInterface* ui) const {
  AvailableMoves moves = ui->QueryMoves(ukey);
  MoveKey mkey = 0;
  AvailableActs acts = ui->QueryActs(ukey, mkey, ActionType::kBasicAttack);
  if (acts.Count() > 0) {
    ui->PushAction(ukey, mkey, ActionType::kBasicAttack, 0 /* Simply choose first one */);
  } else {
    ui->PushAction(ukey, mkey, ActionType::kStay, 0);
  }
}

}  // namespace core
}  // namespace mengde

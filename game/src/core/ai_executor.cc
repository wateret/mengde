#include "ai_executor.h"

#include "user_interface.h"

namespace mengde {
namespace core {

AIExecutor::AIExecutor(UserInterface* ui) : ui_{ui} {
}

unique_ptr<Cmd> AIExecutor::Step() {
  AvailableUnits units = ui_->QueryUnits();

  // A simple rushing AI

  // No more unit waiting for command
  if (units.Count() == 0) {
    return unique_ptr<Cmd>(new CmdEndTurn());
  }

  const UnitKey ukey{0u};

  AvailableMoves moves = ui_->QueryMoves(ukey);

  bool found_target = false;
  MoveKey mkey = 0;
  moves.ForEach([&](const MoveKey& mk, Vec2D) {
    AvailableActs acts = ui_->QueryActs(ukey, mk, ActionType::kBasicAttack);
    if (acts.Count() > 0 && !found_target) {
      found_target = true;
      mkey = mk;
    }
  });

  if (found_target) {
    ui_->PushAction(ukey, mkey, ActionType::kBasicAttack, 0 /* Simply choose first one */);
  } else {
    ui_->PushAction(ukey, GenRandom(moves.Count()), ActionType::kStay, 0);
  }

  return nullptr;
}

}  // namespace core
}  // namespace mengde


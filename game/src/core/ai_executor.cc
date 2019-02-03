#include "ai_executor.h"

#include "ai_unit.h"
#include "user_interface.h"

namespace mengde {
namespace core {

AIExecutor::AIExecutor(UserInterface* ui) : ui_{ui} {}

unique_ptr<Cmd> AIExecutor::Step() {
  AvailableUnits units = ui_->QueryUnits();

  // No more unit waiting for command
  if (units.Count() == 0) {
    return unique_ptr<Cmd>(new CmdEndTurn());
  }

  const UnitKey ukey{0u};

  auto ai_unit = ui_->GetAIUnit(ukey);

  assert(ai_unit);

  ai_unit->play(ukey, ui_);

  return nullptr;
}

}  // namespace core
}  // namespace mengde

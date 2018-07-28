#include "commander.h"
#include "cmd.h"

namespace mengde {
namespace core {

Commander::Commander() : cmdq_current_(new CmdQueue()), cmdq_history_(new CmdQueue()) {}

bool Commander::HasNext() const { return !cmdq_current_->IsEmpty(); }

const Cmd* Commander::GetNextCmdConst() const {
  ASSERT(HasNext());
  return cmdq_current_->GetNextCmdConst();
}

void Commander::DoNext(Stage* game) {
  ASSERT(HasNext());
  auto cmd_done = cmdq_current_->Do(game);
  cmdq_history_->Append(std::move(cmd_done));
}

void Commander::Push(unique_ptr<Cmd> cmd) {
  ///  ASSERT(cmd != nullptr);
  cmdq_current_->Append(std::move(cmd));
}

void Commander::DebugPrint(Stage* stage) const {
#ifdef DEBUG
  cmdq_current_->DebugPrint(stage);
#endif  // DEBUG
}

}  // namespace core
}  // namespace mengde

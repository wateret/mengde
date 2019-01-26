#include "cmd_queue.h"

#include "stage.h"

namespace mengde {
namespace core {

unique_ptr<Cmd> CmdQueue::Do(Stage* game) {
  ASSERT(!IsEmpty());

  /*
  // Processing nested CmdQueue is disabled
  Cmd* current = q_.front().get();
  if (CmdQueue* cq = dynamic_cast<CmdQueue*>(current)) {
    // Remove entry if CmdQueue is empty
    if (cq->IsEmpty()) {
      q_.pop_front();
      return Do(game);
    }
  }
  */

  unique_ptr<Cmd> current = std::move(q_.front());
  q_.pop_front();

  // Nested CmdQueue is not allowed
  ASSERT(dynamic_cast<CmdQueue*>(current.get()) == nullptr);

  unique_ptr<Cmd> result = current->Do(game);
  if (result != nullptr) {
    Prepend(std::move(result));
  }

  if (game->CheckStatus()) {
    switch (game->GetStatus()) {
      case Stage::Status::kUndecided:
        LOG_DEBUG("Undecided!");
        break;
      case Stage::Status::kDefeat:
        LOG_DEBUG("Defeat!");
        break;
      case Stage::Status::kVictory:
        LOG_DEBUG("Victory!");
        game->Push(std::make_unique<CmdGameVictory>());
        break;
      default:
        break;
    }
  }

  return current;
}

void CmdQueue::Insert(unique_ptr<Cmd> cmd, bool prepend) {
  if (cmd == nullptr) return;
  if (CmdQueue* cmdq = dynamic_cast<CmdQueue*>(cmd.get())) {
    if (prepend) {
      // In order to keep the sequence of CmdQueue being appended, push to front in reverse order
      for (auto itr = cmdq->q_.rbegin(); itr != cmdq->q_.rend(); itr++) {
        q_.push_front(std::move(*itr));
      }
    } else {
      for (auto&& e : cmdq->q_) {
        q_.push_back(std::move(e));
      }
    }
  } else {
    if (prepend)
      q_.push_front(std::move(cmd));
    else
      q_.push_back(std::move(cmd));
  }
}

void CmdQueue::Append(unique_ptr<Cmd> cmd) { return Insert(std::move(cmd), false); }

void CmdQueue::Prepend(unique_ptr<Cmd> cmd) { return Insert(std::move(cmd), true); }

CmdQueue& CmdQueue::operator+=(unique_ptr<Cmd> rhs) {
  Append(std::move(rhs));
  return *this;
}

bool CmdQueue::IsEmpty() const {
  return q_.empty();
  /*
  // Processing nested CmdQueue is disabled
  for (auto&& e : q) {
    if (CmdQueue* cmdq = dynamic_cast<CmdQueue*>(e.get())) {
      if (!cmdq->IsEmpty()) return false;
    } else {
      return false;
    }
  }
  */
}

const Cmd* CmdQueue::GetNextCmdConst() const {
  ASSERT(!IsEmpty());

  return q_.front().get();

  /*
  // Processing nested CmdQueue is disabled
  Cmd* next = q_.front().get();
  if (CmdQueue* cq = dynamic_cast<CmdQueue*>(next)) {
    return cq->GetNextAtomicCmd();
  } else {
    return next;
  }
  */
}

}  // namespace core
}  // namespace mengde

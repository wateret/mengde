#include "turn.h"

Turn::Turn() : current_(1), limit_(99), side_(Unit::kSideFirst) {
}

bool Turn::Next() {
  Unit::Side next = static_cast<Unit::Side>(side_ << 1);
  bool next_turn = (next == Unit::kSideLast);
  if (next_turn) {
    next = Unit::kSideFirst;
    current_++;
  }
  side_ = next;
  return next_turn;
}

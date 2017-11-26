#include "turn.h"

Turn::Turn() : no_(0), side_(Unit::kSideFirst) {
}

bool Turn::Next() {
  Unit::Side next = static_cast<Unit::Side>(side_ << 1);
  bool next_turn = (next == Unit::kSideLast);
  if (next_turn) {
    next = Unit::kSideFirst;
    no_++;
  }
  side_ = next;
  return next_turn;
}

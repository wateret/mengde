#include "turn.h"

namespace mengde {
namespace core {

Turn::Turn(uint16_t limit, uint16_t current) : current_{current}, limit_{limit}, force_{Force::kFirst} {}

bool Turn::Next() {
  Force next = static_cast<Force>((uint32_t)force_ << 1);
  bool next_turn = (next == Force::kLast);
  if (next_turn) {
    next = Force::kFirst;
    current_++;
  }
  force_ = next;
  return next_turn;
}

}  // namespace core
}  // namespace mengde

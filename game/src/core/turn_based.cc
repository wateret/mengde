#include "turn_based.h"

#include "util/common.h"

namespace mengde {
namespace core {

TurnBased::TurnBased(uint16_t turn) : turns_left_{turn} {}

void TurnBased::Next() {
  ASSERT(turns_left_ > 0);
  if (turns_left_ != kInfinity) {
    turns_left_--;
  }
}

}  // namespace core
}  // namespace mengde

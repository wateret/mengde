#include "stat_modifier.h"

#include "util/common.h"

namespace mengde {
namespace core {

StatModifier::StatModifier(const std::string& id,
                       uint16_t stat_id,
                       uint16_t addend,
                       uint16_t multiplier,
                       uint16_t turns_left)
    : id_(id),
      stat_id_(stat_id),
      addend_(addend),
      multiplier_(multiplier),
      turns_left_(turns_left) {
}

void StatModifier::NextTurn() {
  ASSERT(turns_left_ > 0);
  if (turns_left_ != kTurnInfinity) {
    turns_left_--;
  }
}

} // namespace core
} // namespace mengde

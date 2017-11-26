#include "stat_modifier.h"

StatModifier::StatModifier(const std::string& id,
                       int16_t stat_id,
                       int16_t addend,
                       int16_t multiplier,
                       int16_t turns_left)
    : id_(id),
      stat_id_(stat_id),
      addend_(addend),
      multiplier_(multiplier),
      turns_left_(turns_left) {
}

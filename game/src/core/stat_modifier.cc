#include "stat_modifier.h"

#include "stat.h"
#include "util/common.h"

namespace mengde {
namespace core {

StatModifier::StatModifier(const std::string& id, uint16_t stat_id, StatMod mod, uint16_t turns_left)
    : id_(id), stat_id_(stat_id), turns_left_(turns_left), mod_(mod) {}

void StatModifier::NextTurn() {
  ASSERT(turns_left_ > 0);
  if (turns_left_ != kTurnInfinity) {
    turns_left_--;
  }
}

string StatModifier::ToString() const {
  string ret;

  //  ret += "[" + id_ + "]";
  //  ret += " ";
  ret += Attribute::kToString[stat_id_];
  ret += " ";
  ret += std::to_string(turns_left_) + "T";
  ret += "/";
  ret += std::to_string(addend()) + "+";
  ret += "/";
  ret += std::to_string(multiplier()) + "%";

  return ret;
}

}  // namespace core
}  // namespace mengde

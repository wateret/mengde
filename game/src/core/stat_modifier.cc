#include "stat_modifier.h"

#include "stat.h"
#include "util/common.h"

namespace mengde {
namespace core {

StatModifier::StatModifier(const std::string& id, uint16_t stat_id, StatMod mod, TurnBased turn)
    : id_(id), stat_id_(stat_id), turn_(turn), mod_(mod) {}

void StatModifier::NextTurn() { turn_.Next(); }

string StatModifier::ToString() const {
  string ret;

  //  ret += "[" + id_ + "]";
  //  ret += " ";
  ret += Attribute::kToString[stat_id_];
  ret += " ";
  ret += std::to_string(turn_.left()) + "T";
  ret += "/";
  ret += std::to_string(addend()) + "+";
  ret += "/";
  ret += std::to_string(multiplier()) + "%";

  return ret;
}

}  // namespace core
}  // namespace mengde

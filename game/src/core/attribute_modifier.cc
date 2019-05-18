#include "attribute_modifier.h"

#include "stat.h"
#include "util/common.h"

namespace mengde {
namespace core {

AttributeModifier::AttributeModifier(const std::string& id, uint16_t stat_id, AttributeChange change, TurnBased turn)
    : id_(id), stat_id_(stat_id), turn_(turn), change_(change) {}

void AttributeModifier::NextTurn() { turn_.Next(); }

string AttributeModifier::ToString() const {
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

#include "equipment.h"

namespace mengde {
namespace core {

Equipment::Equipment(const std::string& id, Type type) : id_(id), type_(type) {
}

void Equipment::AddModifier(StatModifier* sm) {
  unit_effect_list_.AddModifier(sm);
}

void Equipment::AddEffect(EventEffect* ee) {
  unit_effect_list_.AddEffect(ee);
}

Attribute Equipment::CalcAddends() const {
  return unit_effect_list_.CalcAddends();
}

Attribute Equipment::CalcMultipliers() const {
  return unit_effect_list_.CalcMultipliers();
}

void Equipment::RaiseEvent(EventEffect::Type type, Unit* unit) {
  unit_effect_list_.RaiseEvent(type, unit);
}

} // namespace core
} // namespace mengde

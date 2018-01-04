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

Stat Equipment::CalcModifierAddends() const {
  return unit_effect_list_.CalcModifierAddends();
}

Stat Equipment::CalcModifierMultipliers() const {
  return unit_effect_list_.CalcModifierMultipliers();
}

void Equipment::RaiseEvent(EventEffect::Type type, Unit* unit) {
  unit_effect_list_.RaiseEvent(type, unit);
}

} // namespace core
} // namespace mengde

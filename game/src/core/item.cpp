#include "item.h"

Item::Item(const std::string& id,
           ItemType type)
    : id_(id), type_(type) {
}

void Item::AddModifier(StatModifier* sm) {
  unit_effect_list_.AddModifier(sm);
}

void Item::AddEffect(EventEffect* ee) {
  unit_effect_list_.AddEffect(ee);
}

Stat Item::CalcModifierAddends() {
  return unit_effect_list_.CalcModifierAddends();
}

Stat Item::CalcModifierMultipliers() {
  return unit_effect_list_.CalcModifierMultipliers();
}

void Item::RaiseEvent(EventEffect::Type type, Unit* unit) {
  unit_effect_list_.RaiseEvent(type, unit);
}

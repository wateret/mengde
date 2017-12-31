#include "unit_effect_list.h"

UnitEffectList::UnitEffectList() {
}

void UnitEffectList::AddModifier(StatModifier* sm) {
  sm_list_.AddModifier(sm);
}

void UnitEffectList::AddEffect(EventEffect* ee) {
  ee_list_.AddEffect(ee);
}

Stat UnitEffectList::CalcModifierAddends() const {
  return sm_list_.CalcAddends();
}

Stat UnitEffectList::CalcModifierMultipliers() const {
  return sm_list_.CalcMultipliers();
}

void UnitEffectList::RaiseEvent(EventEffect::Type type, Unit* unit) {
  ee_list_.RaiseEvent(type, unit);
}

void UnitEffectList::NextTurn() {
  sm_list_.NextTurn();
  ee_list_.NextTurn();
}

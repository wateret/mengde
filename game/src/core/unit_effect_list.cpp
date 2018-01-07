#include "unit_effect_list.h"

namespace mengde {
namespace core {

UnitEffectList::UnitEffectList() {
}

void UnitEffectList::AddModifier(StatModifier* sm) {
  sm_list_.AddModifier(sm);
}

void UnitEffectList::AddEffect(EventEffect* ee) {
  ee_list_.AddEffect(ee);
}

Attribute UnitEffectList::CalcAddends() const {
  return sm_list_.CalcAddends();
}

Attribute UnitEffectList::CalcMultipliers() const {
  return sm_list_.CalcMultipliers();
}

void UnitEffectList::RaiseEvent(EventEffect::Type type, Unit* unit) {
  ee_list_.RaiseEvent(type, unit);
}

void UnitEffectList::NextTurn() {
  sm_list_.NextTurn();
  ee_list_.NextTurn();
}

} // namespace core
} // namespace mengde

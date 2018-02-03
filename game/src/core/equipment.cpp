#include "equipment.h"

namespace mengde {
namespace core {

Equipment::Equipment(const std::string& id, Type type) : id_(id), type_(type) {
}

void Equipment::AddModifier(StatModifier* sm) {
  modifier_list_.AddModifier(sm);
}

void Equipment::AddEffect(EventEffect* ee) {
  effect_list_.AddEffect(ee);
}

Attribute Equipment::CalcAddends() const {
  return modifier_list_.CalcAddends();
}

Attribute Equipment::CalcMultipliers() const {
  return modifier_list_.CalcMultipliers();
}

void Equipment::RaiseEvent(EventType type, Unit* unit) {
  effect_list_.RaiseEvent(type, unit);
}

} // namespace core
} // namespace mengde

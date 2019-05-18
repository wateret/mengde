#include "equipment.h"

#include "cmd.h"

namespace mengde {
namespace core {

Equipment::Equipment(const std::string& id, Type type) : id_(id), type_(type) {}

void Equipment::AddModifier(AttributeModifier* sm) {
  auto& modifier_list = volatile_attribute_.attribute_modifier_list();
  modifier_list.AddModifier(sm);
}

void Equipment::AddGeneralEffect(GeneralEventEffect* gee) {
  auto& effect_list = volatile_attribute_.event_effect_list();
  effect_list.AddGeneralEffect(gee);
}

void Equipment::AddOnCmdEffect(OnCmdEventEffect* ocee) {
  auto& effect_list = volatile_attribute_.event_effect_list();
  effect_list.AddOnCmdEffect(ocee);
}

Attribute Equipment::CalcAddends() const {
  const auto& modifier_list = volatile_attribute_.attribute_modifier_list();
  return modifier_list.CalcAddends();
}

Attribute Equipment::CalcMultipliers() const {
  const auto& modifier_list = volatile_attribute_.attribute_modifier_list();
  return modifier_list.CalcMultipliers();
}

unique_ptr<Cmd> Equipment::RaiseEvent(event::GeneralEvent type, Unit* unit) const {
  const auto& effect_list = volatile_attribute_.event_effect_list();
  return effect_list.RaiseEvent(type, unit);
}

void Equipment::RaiseEvent(event::OnCmdEvent type, Unit* unit, CmdAct* act) const {
  const auto& effect_list = volatile_attribute_.event_effect_list();
  effect_list.RaiseEvent(type, unit, act);
}

}  // namespace core
}  // namespace mengde

#include "equipment.h"

#include "cmd.h" // TODO Only for CmdQueue

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

void Equipment::AddGeneralEffect(GeneralEventEffect* gee) {
  effect_list_.AddGeneralEffect(gee);
}

void Equipment::AddOnCmdEffect(OnCmdEventEffect* ocee) {
  effect_list_.AddOnCmdEffect(ocee);
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

unique_ptr<Cmd> Equipment::RaiseEvent(event::GeneralEvent type, Unit* unit) const {
  return effect_list_.RaiseEvent(type, unit);
}

void Equipment::RaiseEvent(event::OnCmdEvent type, Unit* unit, CmdAct* act) const {
  effect_list_.RaiseEvent(type, unit, act);
}


} // namespace core
} // namespace mengde

#include "event_effect.h"

#include "cmd.h"
#include "unit.h"

namespace mengde {
namespace core {

// class EventEffectBase

EventEffectBase::EventEffectBase(uint16_t turns_left) : turns_left_(turns_left) {
}

// class GeneralEventEffect

GeneralEventEffect::GeneralEventEffect(event::GeneralEvent type, uint16_t turns_left)
    : EventEffectBase(turns_left), type_(type) {
}

// class GeneralEventEffect

OnCmdEventEffect::OnCmdEventEffect(event::OnCmdEvent type, uint16_t turns_left)
    : EventEffectBase(turns_left), type_(type) {
}

// GeneralEventEffect Derivatives

// class GEERestoreHp

GEERestoreHp::GEERestoreHp(event::GeneralEvent type, int multiplier, int addend, uint16_t turns_left)
    : GeneralEventEffect(type, turns_left), multiplier_(multiplier), addend_(addend) {
}

unique_ptr<Cmd> GEERestoreHp::OnEvent(Unit* unit) {
  return unique_ptr<Cmd>(new CmdRestoreHp(unit, multiplier_, addend_));
}

// OnCmdEventEffect Derivatives

// class OCEEPreemptiveAttack

OCEEPreemptiveAttack::OCEEPreemptiveAttack(event::OnCmdEvent type, uint16_t turns_left)
    : OnCmdEventEffect(type, turns_left) {
}

void OCEEPreemptiveAttack::OnEvent(Unit* unit, CmdAct* act) {
  LOG_INFO("'%s' Preemptive Attack activated.", unit->GetId().c_str());

  // Must check if the unit is defender to prevent from effect duplication.
  //  Applying this effect twice will swap back the attacker and defender.
  if (act->GetUnitDef() == unit) {
    act->SwapAtkDef();
  }
}

// class OCEEEnhanceBasicAttack
OCEEEnhanceBasicAttack::OCEEEnhanceBasicAttack(event::OnCmdEvent type, int multiplier, int addend, uint16_t turns_left)
    : OnCmdEventEffect(type, turns_left), multiplier_(multiplier), addend_(addend) {
}

void OCEEEnhanceBasicAttack::OnEvent(Unit* unit, CmdAct* act) {
  LOG_INFO("'%s' the damage will be enhanced by (%d%%,+%d)", unit->GetId().c_str(), multiplier_, addend_);
  CmdBasicAttack* ba = dynamic_cast<CmdBasicAttack*>(act);
  ba->AddToMultiplier(multiplier_);
  ba->AddToAddend(addend_);
}

} // namespace core
} // namespace mengde

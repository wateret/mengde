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

GEERestoreHp::GEERestoreHp(event::GeneralEvent type, int ratio, int adder, uint16_t turns_left)
    : GeneralEventEffect(type, turns_left), ratio_(ratio), adder_(adder) {
  ASSERT(0 <= ratio_ && ratio_ <= 100);
}

unique_ptr<Cmd> GEERestoreHp::OnEvent(Unit* unit) {
  return unique_ptr<Cmd>(new CmdRestoreHp(unit, ratio_, adder_));
}

// OnCmdEventEffect Derivatives

// class OCEEPreemptiveAttack

OCEEPreemptiveAttack::OCEEPreemptiveAttack(event::OnCmdEvent type, uint16_t turns_left)
    : OnCmdEventEffect(type, turns_left) {
}

void OCEEPreemptiveAttack::OnEvent(Unit* unit, CmdAct* act) {
  LOG_INFO("Preemptive Attack activated.");

  // Must check if the unit is defender to prevent from effect duplication.
  //  Applying this effect twice will swap back the attacker and defender.
  if (act->GetUnitDef() == unit) {
    act->SwapAtkDef();
  }
}

} // namespace core
} // namespace mengde

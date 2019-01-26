#include "event_effect.h"

#include "cmds.h"
#include "unit.h"

namespace mengde {
namespace core {

// class EventEffectBase

EventEffectBase::EventEffectBase(TurnBased turn) : turn_{turn} {}

void EventEffectBase::NextTurn() { turn_.Next(); }

// class GeneralEventEffect

GeneralEventEffect::GeneralEventEffect(event::GeneralEvent type, TurnBased turn) : EventEffectBase(turn), type_(type) {}

// class GeneralEventEffect

OnCmdEventEffect::OnCmdEventEffect(event::OnCmdEvent type, TurnBased turn) : EventEffectBase(turn), type_(type) {}

// GeneralEventEffect Derivatives

// class GEERestoreHp

GEERestoreHp::GEERestoreHp(event::GeneralEvent type, int multiplier, int addend, TurnBased turn)
    : GeneralEventEffect(type, turn), multiplier_(multiplier), addend_(addend) {}

unique_ptr<Cmd> GEERestoreHp::OnEvent(Unit* unit) {
  return unique_ptr<Cmd>(new CmdRestoreHp(unit->uid(), multiplier_, addend_));
}

// OnCmdEventEffect Derivatives

// class OCEEPreemptiveAttack

OCEEPreemptiveAttack::OCEEPreemptiveAttack(event::OnCmdEvent type, TurnBased turn) : OnCmdEventEffect(type, turn) {}

void OCEEPreemptiveAttack::OnEvent(Unit* unit, CmdAct* act) {
  LOG_INFO("'%s' Preemptive Attack activated.", unit->id().c_str());

  // Must check if the unit is defender to prevent from effect duplication.
  //  Applying this effect twice will swap back the attacker and defender.
  if (act->GetUnitDef() == unit->uid()) {
    act->SwapAtkDef();
  }
}

// class OCEEEnhanceBasicAttack
OCEEEnhanceBasicAttack::OCEEEnhanceBasicAttack(event::OnCmdEvent type, int multiplier, int addend, TurnBased turn)
    : OnCmdEventEffect(type, turn), multiplier_(multiplier), addend_(addend) {}

void OCEEEnhanceBasicAttack::OnEvent(Unit* unit, CmdAct* act) {
  LOG_INFO("'%s' the damage will be enhanced by (%d%%,+%d)", unit->id().c_str(), multiplier_, addend_);
  CmdBasicAttack* ba = dynamic_cast<CmdBasicAttack*>(act);
  ASSERT(ba != nullptr);
  ba->AddToMultiplier(multiplier_);
  ba->AddToAddend(addend_);
}

}  // namespace core
}  // namespace mengde

#include "event_effect.h"

#include "cmds.h"
#include "unit.h"
#include "attribute_modifier.h"

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

GEERestoreHp::GEERestoreHp(event::GeneralEvent type, AttributeChange change, TurnBased turn)
    : GeneralEventEffect{type, turn}, change_{change} {}

unique_ptr<Cmd> GEERestoreHp::OnEvent(Unit* unit) {
  return unique_ptr<Cmd>(new CmdRestoreHp{unit->uid(), change_});
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
OCEEEnhanceBasicAttack::OCEEEnhanceBasicAttack(event::OnCmdEvent type, AttributeChange change, TurnBased turn)
    : OnCmdEventEffect{type, turn}, change_{change} {}

void OCEEEnhanceBasicAttack::OnEvent(Unit* unit, CmdAct* act) {
  LOG_INFO("'%s' the damage will be enhanced by (%d%%,+%d)", unit->id().c_str(), change_.multiplier, change_.addend);
  CmdBasicAttack* ba = dynamic_cast<CmdBasicAttack*>(act);
  ASSERT(ba != nullptr);
  ba->UpdateChange(change_);
}

}  // namespace core
}  // namespace mengde

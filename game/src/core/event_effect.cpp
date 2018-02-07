#include "event_effect.h"

#include "cmd.h"
#include "unit.h"

namespace mengde {
namespace core {

// class EventEffect

EventEffect::~EventEffect() {
}

EventEffect::EventEffect(EventType type, uint16_t turns_left) : type_(type), turns_left_(turns_left) {
}

void EventEffect::NextTurn() {
  ASSERT(turns_left_ > 0);
  if (turns_left_ != kTurnInfinity) {
    turns_left_--;
  }
}

// class EERestoreHP

EERestoreHP::EERestoreHP(EventType type, int amount)
    : EventEffect(type), amount_(amount) {
  ASSERT(0 <= amount_ && amount_ <= 100);
}

void EERestoreHP::OnEvent(Unit* unit) {
  int maxhp = unit->GetOriginalHpMp().hp;
  int real_amount = maxhp * amount_ / 100;
  unit->RestoreHP(real_amount);
  LOG_INFO("EERestoreHP restores HP by %d", real_amount);
}

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

GEERestoreHp::GEERestoreHp(event::GeneralEvent type, int ratio, int adder, uint16_t turns_left)
    : GeneralEventEffect(type, turns_left), ratio_(ratio), adder_(adder) {
  ASSERT(0 <= ratio_ && ratio_ <= 100);
}

unique_ptr<Cmd> GEERestoreHp::OnEvent(Unit* unit) {
  return unique_ptr<Cmd>(new CmdRestoreHp(unit, ratio_, adder_));
}

} // namespace core
} // namespace mengde

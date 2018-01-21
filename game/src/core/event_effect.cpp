#include "event_effect.h"

#include "unit.h"

namespace mengde {
namespace core {

// class EventEffect

EventEffect::~EventEffect() {
}

EventEffect::EventEffect(Type type, uint16_t turns_left) : type_(type), turns_left_(turns_left) {
}

void EventEffect::NextTurn() {
  ASSERT(turns_left_ > 0);
  if (turns_left_ != kTurnInfinity) {
    turns_left_--;
  }
}

// class EERestoreHP

EERestoreHP::EERestoreHP(Type type, int amount)
    : EventEffect(type), amount_(amount) {
  ASSERT(0 <= amount_ && amount_ <= 100);
}

void EERestoreHP::OnEvent(Unit* unit) {
  int maxhp = unit->GetOriginalHpMp().hp;
  int real_amount = maxhp * amount_ / 100;
  unit->RestoreHP(real_amount);
  LOG_INFO("EERestoreHP restores HP by %d", real_amount);
}

} // namespace core
} // namespace mengde

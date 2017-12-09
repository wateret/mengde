#include "event_effect.h"

#include "unit.h"

// class EventEffect

EventEffect::~EventEffect() {
}

EventEffect::EventEffect(Type type) : type_(type) {
}

// class EERestoreHP

EERestoreHP::EERestoreHP(Type type, int amount)
    : EventEffect(type), amount_(amount) {
  ASSERT(0 <= amount_ && amount_ <= 100);
}

void EERestoreHP::OnEvent(Unit* unit) {
  int maxhp = unit->GetOriginalXtat()->hp;
  int real_amount = maxhp * amount_ / 100;
  unit->RestoreHP(real_amount);
  LOG_INFO("EERestoreHP restores HP by %d", real_amount);
}


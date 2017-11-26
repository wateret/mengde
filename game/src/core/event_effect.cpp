#include "event_effect.h"

#include "unit.h"

// class EventEffect

EventEffect::~EventEffect() {
}

EventEffect::EventEffect(EventEffectType type) : type_(type) {
}

// class EERestoreHPAfterAction

EERestoreHPAfterAction::EERestoreHPAfterAction(int amount)
    : EventEffect(kOnActionDone), amount_(amount) {
  ASSERT(0 <= amount_ && amount_ <= 100);
}

void EERestoreHPAfterAction::OnEvent(Unit* unit, void*) {
  int maxhp = unit->GetOriginalXtat()->hp;
  int real_amount = maxhp * amount_ / 100;
  unit->RestoreHP(real_amount);
  LOG_INFO("EERestoreHPAfterAction restores HP by %d", real_amount);
}

// class EERestoreOnTurnBegin

EERestoreHPOnTurnBegin::EERestoreHPOnTurnBegin(int amount)
    : EventEffect(kOnTurnBegin), amount_(amount) {
  ASSERT(0 <= amount_ && amount_ <= 100);
}

void EERestoreHPOnTurnBegin::OnEvent(Unit* unit, void*) {
  int maxhp = unit->GetOriginalXtat()->hp;
  int real_amount = maxhp * amount_ / 100;
  unit->RestoreHP(real_amount);
  LOG_INFO("EERestoreHPOnTurnBegin restores HP by %d", real_amount);
}

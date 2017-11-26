#ifndef UNIT_EFFECT_LIST_H_
#define UNIT_EFFECT_LIST_H_

#include "stat_modifier_list.h"
#include "event_effect_list.h"

class UnitEffectList {
 public:
  UnitEffectList();
  void AddModifier(StatModifier*);
  Stat CalcModifierAddends();
  Stat CalcModifierMultipliers();
  void AddEffect(EventEffect*);
  void RaiseEvent(EventEffect::EventEffectType, Unit*, void*);
  void NextTurn();

 private:
  StatModifierList sm_list_;
  EventEffectList  ee_list_;
};

#endif // UNIT_EFFECT_LIST_H_

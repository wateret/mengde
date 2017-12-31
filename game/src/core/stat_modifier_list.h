#ifndef STAT_MODIFIER_LIST_H_
#define STAT_MODIFIER_LIST_H_

#include <vector>
#include "stat.h"

class StatModifier;

class StatModifierList {
 public:
  StatModifierList();
  ~StatModifierList();
  void AddModifier(StatModifier*);
  void NextTurn();
  Stat CalcAddends() const;
  Stat CalcMultipliers() const;

 private:
  std::vector<StatModifier*> elements_;
};

#endif // STAT_MODIFIER_LIST_H_

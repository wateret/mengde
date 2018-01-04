#ifndef STAT_MODIFIER_LIST_H_
#define STAT_MODIFIER_LIST_H_

#include <vector>

#include "stat.h"

namespace mengde {
namespace core {

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

} // namespace core
} // namespace mengde

#endif // STAT_MODIFIER_LIST_H_

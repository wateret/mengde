#ifndef MENGDE_CORE_STAT_MODIFIER_LIST_H_
#define MENGDE_CORE_STAT_MODIFIER_LIST_H_

#include <vector>

#include "stat.h"

namespace mengde {
namespace core {

class StatModifier;

class StatModifierList {
 public:
  StatModifierList();
  ~StatModifierList();
  void      AddModifier(StatModifier*);
  void      NextTurn();
  Attribute CalcAddends() const;
  Attribute CalcMultipliers() const;

 private:
  std::vector<StatModifier*> elements_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_STAT_MODIFIER_LIST_H_

#ifndef MENGDE_CORE_ATTRIBUTE_MODIFIER_LIST_H_
#define MENGDE_CORE_ATTRIBUTE_MODIFIER_LIST_H_

#include <functional>
#include <vector>

#include "stat.h"

namespace mengde {
namespace core {

class AttributeModifier;

class AttributeModifierList {
 public:
  AttributeModifierList();
  ~AttributeModifierList();
  void AddModifier(AttributeModifier *);
  void NextTurn();
  Attribute CalcAddends() const;
  Attribute CalcMultipliers() const;
  void iterate(const std::function<void(const AttributeModifier &)> &fn) const;

 private:
  std::vector<AttributeModifier *> elements_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_ATTRIBUTE_MODIFIER_LIST_H_

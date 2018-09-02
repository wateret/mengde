#ifndef MENGDE_CORE_HERO_TEMPLATE_H_
#define MENGDE_CORE_HERO_TEMPLATE_H_

#include "stat.h"
#include "unit_class.h"
#include "util/common.h"

namespace mengde {
namespace core {

class HeroTemplate {
 public:
  HeroTemplate(const string& id, const UnitClass* unit_class, const Attribute& hero_attr);
  string id() const { return id_; }
  const UnitClass* unit_class() const { return unit_class_; }
  int class_index() const;
  int move() const;
  const Attribute& GetHeroStat() const { return hero_attr_; }
  const AttackRange& attack_range() const;

 private:
  string id_;
  const UnitClass* unit_class_;
  Attribute hero_attr_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_HERO_TEMPLATE_H_

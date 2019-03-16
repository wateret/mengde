#ifndef MENGDE_CORE_HERO_TEMPLATE_H_
#define MENGDE_CORE_HERO_TEMPLATE_H_

#include "hero_class.h"
#include "stat.h"
#include "util/common.h"

namespace mengde {
namespace core {

class HeroTemplate {
 public:
  HeroTemplate(const string& id, const HeroClass* hero_class, const Attribute& hero_attr);
  string id() const { return id_; }
  const HeroClass* hero_class() const { return hero_class_; }
  int class_index() const;
  int move() const;
  const Attribute& GetHeroStat() const { return hero_attr_; }
  const AttackRange& attack_range() const;

 private:
  string id_;
  const HeroClass* hero_class_;
  Attribute hero_attr_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_HERO_TEMPLATE_H_

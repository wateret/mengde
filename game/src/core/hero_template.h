#ifndef HERO_TEMPLATE_H_
#define HERO_TEMPLATE_H_

#include "stat.h"
#include "unit_class.h"
#include "util/common.h"

namespace mengde {
namespace core {

class HeroTemplate {
 public:
  HeroTemplate(const string&, const string&, const UnitClass*, const Attribute&);
  string           GetId() const { return id_; }
  string           GetModelId() const { return bitmap_path_; }
  const UnitClass* GetClass() const { return unit_class_; }
  int              GetClassIndex() const;
  int              GetMove() const;
  const Attribute& GetHeroStat() const { return hero_attr_; }
  Vec2D*           GetAttackRange() const;

 private:
  string           id_;
  string           bitmap_path_;
  const UnitClass* unit_class_;
  Attribute        hero_attr_;
};

}  // namespace core
}  // namespace mengde

#endif  // HERO_TEMPLATE_H_

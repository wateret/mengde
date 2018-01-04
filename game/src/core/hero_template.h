#ifndef HERO_TEMPLATE_H_
#define HERO_TEMPLATE_H_

#include "util/common.h"
#include "stat.h"
#include "unit_class.h"

namespace mengde {
namespace core {

class HeroTemplate {
 public:
  HeroTemplate(const string&, const string&, const UnitClass*, const Stat&);
  string GetId() const { return id_; }
  string GetBitmapPath() const { return bitmap_path_; }
  const UnitClass* GetClass() const { return unit_class_; }
  int GetClassIndex() const;
  int GetMove() const;
  const Stat& GetHeroStat() const { return hero_stat_; }
  Vec2D*      GetAttackRange() const;

 private:
  string           id_;
  string           bitmap_path_;
  const UnitClass* unit_class_;
  Stat             hero_stat_;
};

} // namespace core
} // namespace mengde

#endif // HERO_TEMPLATE_H_

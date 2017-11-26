#include "hero.h"

Hero::Hero(const std::string& id,
           const std::string& bitmap_path,
           const UnitClass* unit_class,
           int level,
           const Stat& hero_stat)
    : id_(id),
      bitmap_path_(bitmap_path),
      unit_class_(unit_class),
      level_(level),
      hero_stat_(hero_stat),
      unit_stat_(),
      xtat_() {
  UpdateUnitStat();
}

int Hero::GetClassIndex() const {
  return unit_class_->GetIndex();
}

int Hero::GetMove() const {
  return unit_class_->GetMove();
}

Vec2D* Hero::GetAttackRange() const {
  return unit_class_->GetAttackRange();
}

void Hero::LevelUp() {
  level_++;
  UpdateUnitStat();
}

void Hero::UpdateUnitStat() {
#define UpdateXtat(x, xc) xtat_.x = unit_class_->GetBni##xc().base + \
                                    unit_class_->GetBni##xc().incr * level_
#define UpdateStat(x) unit_stat_.x = ((hero_stat_.x / 2) + \
                                      ((100 + 8 * (unit_class_->GetStatGrade()->x - 1)) * level_ * hero_stat_.x) / 3000)
  // TODO UpdateStat(x) formula should depend on its unitclass grade
  UpdateXtat(hp, Hp);
  UpdateXtat(mp, Mp);
  UpdateStat(atk);
  UpdateStat(def);
  UpdateStat(dex);
  UpdateStat(itl);
  UpdateStat(mor);
#undef UpdateXtat
#undef UpdateStat
}


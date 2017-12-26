#include "hero.h"

Hero::Hero(const HeroTemplate* hero_tpl, uint16_t level)
    : hero_tpl_(hero_tpl), level_(level), hero_stat_(hero_tpl->GetHeroStat()), unit_stat_(), xtat_() {
  UpdateUnitStat();
}

string Hero::GetId() const {
  return hero_tpl_->GetId();
}

string Hero::GetBitmapPath() const {
  return hero_tpl_->GetBitmapPath();
}

const UnitClass* Hero::GetClass() const {
  return hero_tpl_->GetClass();
}

int Hero::GetClassIndex() const {
  return hero_tpl_->GetClassIndex();
}

int Hero::GetMove() const {
  return hero_tpl_->GetMove();
}

Vec2D* Hero::GetAttackRange() const {
  return hero_tpl_->GetAttackRange();
}

const Stat& Hero::GetHeroStatBase() const {
  return hero_tpl_->GetHeroStat();
}

void Hero::LevelUp() {
  level_++;
  UpdateUnitStat();
}

void Hero::UpdateUnitStat() {
#define UpdateXtat(x, xc) xtat_.x = GetClass()->GetBni##xc().base + \
                                    GetClass()->GetBni##xc().incr * level_
#define UpdateStat(x) unit_stat_.x = ((hero_stat_.x / 2) + \
                                      ((100 + 8 * (GetClass()->GetStatGrade()->x - 1)) * level_ * hero_stat_.x) / 3000)
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


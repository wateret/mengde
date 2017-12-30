#ifndef HERO_H_
#define HERO_H_

#include "util/common.h"
#include "stat.h"
#include "unit_class.h"
#include "hero_template.h"

class Hero {
 public:
  Hero(const HeroTemplate*, uint16_t);
  string GetId() const;
  string GetBitmapPath() const; // TODO Rename this to GetModelId
  const UnitClass* GetClass() const;
  int GetClassIndex() const;
  int GetMove() const;
  const Stat& GetHeroStat() const { return hero_stat_; }
  Vec2D*      GetAttackRange() const;

  const Stat& GetHeroStatBase() const;
  uint16_t    GetLevel()    const { return level_; }
  const Stat& GetUnitStat() const { return unit_stat_; }
  const Xtat& GetXtat()     const { return xtat_; }
  void        LevelUp();

 private:
  void UpdateUnitStat();

 private:
  const HeroTemplate* hero_tpl_;
  uint16_t level_;
  Stat     hero_stat_;
  Stat     unit_stat_;
  Xtat     xtat_;
};

#endif // HERO_H_

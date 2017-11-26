#ifndef HERO_H_
#define HERO_H_

#include "stat.h"
#include "unit_class.h"

class Hero {
 public:
  Hero(const std::string&,
       const std::string&,
       const UnitClass*,
       int,
       const Stat&);
  std::string GetId() const { return id_; }
  std::string GetBitmapPath() const { return bitmap_path_; }
  const UnitClass* GetClass() const { return unit_class_; }
  int GetLevel() const { return level_; }
  const Stat* GetHeroStat() const { return &hero_stat_; }
  const Stat* GetUnitStat() const { return &unit_stat_; }
  const Xtat* GetXtat() const { return &xtat_; }
  int GetMove() const;
  int GetClassIndex() const;
  Vec2D* GetAttackRange() const;
  void LevelUp();

 private:
  void UpdateUnitStat();

 private:
  std::string      id_;
  std::string      bitmap_path_;
  const UnitClass* unit_class_;
  int              level_;
  Stat             hero_stat_;
  Stat             unit_stat_;
  Xtat             xtat_;
};

#endif // HERO_H_

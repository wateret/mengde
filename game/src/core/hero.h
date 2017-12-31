#ifndef HERO_H_
#define HERO_H_

#include "util/common.h"
#include "stat.h"
#include "unit_class.h"
#include "hero_template.h"
#include "i_equipper.h"

class EquipmentSet;

class Hero : public IEquipper {
 public:
  Hero(const HeroTemplate*, uint16_t);
  ~Hero();
  string GetId() const;
  string GetBitmapPath() const; // TODO Rename this to GetModelId
  const UnitClass* GetClass() const override;
  int GetClassIndex() const;
  int GetMove() const;
  const Stat& GetHeroStat() const { return hero_stat_; }
  Vec2D*      GetAttackRange() const;
  EquipmentSet* GetEquipmentSet() { return equipment_set_; }

  const Stat& GetHeroStatBase() const;
  uint16_t    GetLevel()    const { return level_; }
  const Stat& GetUnitStat() const { return unit_stat_; }
  const Xtat& GetXtat()     const { return xtat_; }
  void        LevelUp();

  Xtat CalcXtat() const;
  Stat CalcUnitStat() const;
  void UpdateStat() override;

 private:
  const HeroTemplate* hero_tpl_;
  EquipmentSet* equipment_set_;
  uint16_t level_;
  Stat     hero_stat_;
  Stat     unit_stat_;
  Xtat     xtat_;
};

#endif // HERO_H_

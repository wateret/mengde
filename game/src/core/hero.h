#ifndef HERO_H_
#define HERO_H_

#include "util/common.h"
#include "stat.h"
#include "unit_class.h"
#include "hero_template.h"
#include "i_equipper.h"

namespace mengde {
namespace core {

class EquipmentSet;
class Equipment;

class Hero : public IEquipper {
 public:
  Hero(const HeroTemplate*, uint16_t);
  ~Hero();
  string GetId() const;
  string GetBitmapPath() const; // TODO Rename this to GetModelId
  const UnitClass* GetClass() const override;
  int GetClassIndex() const;
  int GetMove() const;
  const Attribute& GetHeroStat() const { return hero_stat_; }
  Vec2D*      GetAttackRange() const;
  const EquipmentSet* GetEquipmentSet() const { return equipment_set_; }

  const Attribute& GetHeroStatBase() const;
  uint16_t GetLevel() const { return level_.level; }
  uint16_t GetExp()   const { return level_.exp; }
  void     GainExp(uint16_t exp) { level_.exp += exp; }
  const Attribute& GetUnitStat() const { return unit_stat_; }
  const HpMp& GetHpMp()     const { return xtat_; }
  void        LevelUp();
  void        PutOn(const Equipment*);

  HpMp CalcHpMp() const;
  Attribute CalcUnitStat() const;
  Attribute CalcPureUnitStat() const;
  void UpdateStat() override;

 private:
  const HeroTemplate* hero_tpl_;
  EquipmentSet* equipment_set_;
  Level     level_;
  Attribute hero_stat_;
  Attribute unit_stat_;
  HpMp      xtat_;
};

} // namespace core
} // namespace mengde

#endif // HERO_H_

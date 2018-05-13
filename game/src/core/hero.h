#ifndef MENGDE_CORE_HERO_H_
#define MENGDE_CORE_HERO_H_

#include "hero_template.h"
#include "i_equipper.h"
#include "i_unit_base.h"
#include "stat.h"
#include "unit_class.h"
#include "util/common.h"

namespace mengde {
namespace core {

class EquipmentSet;
class Equipment;

class Hero : public IUnitBase, IEquipper {
 public:
  Hero(const HeroTemplate*, uint16_t);
  Hero(const Hero&);
  virtual ~Hero();

 public:
  // IUnitBase interfaces
  virtual string GetId() const override;
  virtual string GetModelId() const override;
  virtual const UnitClass* GetClass() const override;
  virtual int GetMove() const override;
  virtual const AttackRange& GetAttackRange() const override;
  virtual uint16_t GetLevel() const override { return level_.level; }
  virtual uint16_t GetExp() const override { return level_.exp; }
  virtual const HpMp& GetOriginalHpMp() const override { return hpmp_; }
  virtual const Attribute& GetOriginalAttr() const override { return unit_pure_attr_; }
  virtual const HpMp& GetCurrentHpMp() const override { return GetOriginalHpMp(); }
  virtual const Attribute& GetCurrentAttr() const override { return unit_attr_; }
  virtual const EquipmentSet* GetEquipmentSet() const override { return equipment_set_; }
  virtual void UpdateStat() override;

  const Attribute& GetHeroStatBase() const;
  const Attribute& GetHeroStat() const { return hero_attr_; }
  void GainExp(uint16_t exp) { level_.exp += exp; }
  const Attribute& GetUnitPureStat() const { return unit_pure_attr_; }
  void LevelUp();
  void PutOn(const Equipment*);
  int GetClassIndex() const;

 private:
  HpMp CalcHpMp() const;
  Attribute CalcUnitPureAttr() const;

 private:
  const HeroTemplate* hero_tpl_;
  EquipmentSet* equipment_set_;
  Level level_;
  Attribute hero_attr_;
  Attribute unit_attr_;
  Attribute unit_pure_attr_;  // Cached data
  HpMp hpmp_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_HERO_H_

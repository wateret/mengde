#ifndef MENGDE_CORE_HERO_H_
#define MENGDE_CORE_HERO_H_

#include "hero_class.h"
#include "hero_template.h"
#include "i_equipper.h"
#include "i_unit_base.h"
#include "resource_manager.h"
#include "stat.h"
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
  virtual string id() const override;
  virtual const HeroClass* unit_class() const override { return unit_class_; }
  virtual int move() const override;
  virtual const AttackRange& attack_range() const override;
  virtual uint16_t GetLevel() const override { return level_.level; }
  virtual uint16_t GetExp() const override { return level_.exp; }
  virtual const HpMp& GetOriginalHpMp() const override { return hpmp_; }
  virtual const Attribute& GetOriginalAttr() const override { return unit_pure_attr_; }
  virtual const HpMp& GetCurrentHpMp() const override { return GetOriginalHpMp(); }
  virtual const Attribute& GetCurrentAttr() const override { return unit_attr_; }
  virtual const EquipmentSet* GetEquipmentSet() const override { return equipment_set_.get(); }
  virtual void UpdateStat() override;

  const Attribute& GetHeroStatBase() const;
  const Attribute& GetHeroStat() const { return hero_attr_; }
  void GainExp(uint16_t exp) { level_.exp += exp; }
  bool IsExpFull() const { return level_.exp >= Level::kExpLimit; }
  const Attribute& GetUnitPureStat() const { return unit_pure_attr_; }
  void LevelUp();
  bool ReadyPromotion() const;
  void Promote(const UnitClassManager* ucm);
  void PutOn(const Equipment*);
  int class_index() const;

 private:
  HpMp CalcHpMp() const;
  Attribute CalcUnitPureAttr() const;

 private:
  const HeroTemplate* hero_tpl_;
  const HeroClass* unit_class_;
  unique_ptr<EquipmentSet> equipment_set_;
  Level level_;
  Attribute hero_attr_;
  Attribute unit_attr_;
  Attribute unit_pure_attr_;  // Cached data
  HpMp hpmp_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_HERO_H_

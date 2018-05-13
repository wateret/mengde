#include "hero.h"

#include "equipment.h"
#include "equipment_set.h"

namespace mengde {
namespace core {

Hero::Hero(const HeroTemplate* hero_tpl, uint16_t level)
    : hero_tpl_(hero_tpl),
      equipment_set_(new EquipmentSet(this)),
      level_(level, 0),
      hero_attr_(hero_tpl->GetHeroStat()),
      unit_attr_(),
      unit_pure_attr_(),
      hpmp_() {
  UpdateStat();
}

Hero::Hero(const Hero& hero)
    : hero_tpl_(hero.hero_tpl_),
      equipment_set_(hero.GetEquipmentSet()->Clone(this)),
      level_(hero.level_),
      hero_attr_(hero.hero_attr_),
      unit_attr_(hero.unit_attr_),
      unit_pure_attr_(hero.unit_pure_attr_),
      hpmp_(hero.hpmp_) {}

Hero::~Hero() { delete equipment_set_; }

string Hero::GetId() const { return hero_tpl_->GetId(); }

string Hero::GetModelId() const { return hero_tpl_->GetModelId(); }

const UnitClass* Hero::GetClass() const { return hero_tpl_->GetClass(); }

int Hero::GetClassIndex() const { return hero_tpl_->GetClassIndex(); }

int Hero::GetMove() const { return hero_tpl_->GetMove(); }

const AttackRange& Hero::GetAttackRange() const { return hero_tpl_->GetAttackRange(); }

const Attribute& Hero::GetHeroStatBase() const { return hero_tpl_->GetHeroStat(); }

void Hero::LevelUp() {
  level_.level++;
  UpdateStat();
}

void Hero::PutOn(const Equipment* equipment) { equipment_set_->SetEquipment(equipment); }

HpMp Hero::CalcHpMp() const {
  HpMp xtat;
#define UPDATE_HPMP(x, xc) xtat.x = GetClass()->GetBni##xc().base + GetClass()->GetBni##xc().incr * level_.level
  UPDATE_HPMP(hp, Hp);
  UPDATE_HPMP(mp, Mp);
#undef UPDATE_HPMP
  return xtat;
}

Attribute Hero::CalcUnitPureAttr() const {
  Attribute unit_stat =
      ((hero_attr_ / 2) + ((100 + 10 * (GetClass()->GetStatGrade() - 1)) * level_.level * hero_attr_) / 2000);
  return unit_stat;
}

void Hero::UpdateStat() {
  hpmp_ = CalcHpMp();
  unit_pure_attr_ = CalcUnitPureAttr();
  unit_attr_ = unit_pure_attr_;
  {  // FIXME code copied from Unit
    Attribute addends = equipment_set_->CalcAddends();
    Attribute multipliers = equipment_set_->CalcMultipliers();

    unit_attr_.ApplyModifier(multipliers, addends);
  }
}

}  // namespace core
}  // namespace mengde

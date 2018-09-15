#include "hero.h"

#include "equipment.h"
#include "equipment_set.h"

namespace mengde {
namespace core {

Hero::Hero(const HeroTemplate* hero_tpl, uint16_t level)
    : hero_tpl_(hero_tpl),
      unit_class_{hero_tpl_->unit_class()},
      equipment_set_(new EquipmentSet(this)),
      level_(level, 0),
      hero_attr_(hero_tpl->GetHeroStat()),
      unit_attr_(),
      unit_pure_attr_(),
      hpmp_() {
  ASSERT(unit_class_ != nullptr);
  // TODO Handle promotion chain
  UpdateStat();
}

Hero::Hero(const Hero& hero)
    : hero_tpl_(hero.hero_tpl_),
      unit_class_{hero.unit_class()},
      equipment_set_{new EquipmentSet{this}},
      level_(hero.level_),
      hero_attr_(hero.hero_attr_),
      unit_attr_(hero.unit_attr_),
      unit_pure_attr_(hero.unit_pure_attr_),
      hpmp_(hero.hpmp_) {
  ASSERT(unit_class_ != nullptr);
}

Hero::~Hero() {}

string Hero::id() const { return hero_tpl_->id(); }

int Hero::class_index() const { return unit_class_->index(); }

int Hero::move() const { return unit_class_->move(); }

const AttackRange& Hero::attack_range() const { return unit_class_->attack_range(); }

const Attribute& Hero::GetHeroStatBase() const { return hero_tpl_->GetHeroStat(); }

void Hero::LevelUp() {
  ASSERT(IsExpFull());
  ASSERT(level_.exp >= Level::kExpLimit);

  level_.exp -= Level::kExpLimit;
  level_.level++;

  UpdateStat();
}

void Hero::PutOn(const Equipment* equipment) { equipment_set_->SetEquipment(equipment); }

HpMp Hero::CalcHpMp() const {
  HpMp hpmp;
#define UPDATE_HPMP(x) hpmp.x = unit_class()->bni_##x().base + unit_class()->bni_##x().incr * level_.level
  UPDATE_HPMP(hp);
  UPDATE_HPMP(mp);
#undef UPDATE_HPMP
  return hpmp;
}

Attribute Hero::CalcUnitPureAttr() const {
  Attribute unit_stat =
      ((hero_attr_ / 2) + ((100 + 10 * (unit_class()->stat_grade() - 1)) * level_.level * hero_attr_) / 2000);
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

bool Hero::ReadyPromotion() const {
  auto promotion_info = unit_class_->promotion_info();
  if (promotion_info) {
    return GetLevel() >= promotion_info->level;
  }
  return false;
}

void Hero::Promote(const UnitClassManager* ucm) {
  ASSERT(ReadyPromotion());

  auto promotion_info = unit_class_->promotion_info();
  auto unit_class_id = promotion_info->id;
  unit_class_ = ucm->Get(unit_class_id);
  ASSERT(unit_class_ != nullptr);

  UpdateStat();
}

}  // namespace core
}  // namespace mengde

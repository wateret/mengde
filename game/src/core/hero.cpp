#include "hero.h"

#include "equipment_set.h"
#include "equipment.h"

namespace mengde {
namespace core {

Hero::Hero(const HeroTemplate* hero_tpl, uint16_t level)
    : hero_tpl_(hero_tpl),
      equipment_set_(new EquipmentSet(this)),
      level_(level, 0),
      hero_stat_(hero_tpl->GetHeroStat()),
      unit_stat_(),
      xtat_() {
  unit_stat_ = CalcUnitStat();
  xtat_ = CalcHpMp();
  UpdateStat();
}

Hero::~Hero() {
  delete equipment_set_;
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

const Attribute& Hero::GetHeroStatBase() const {
  return hero_tpl_->GetHeroStat();
}

void Hero::LevelUp() {
  level_.level++;
  unit_stat_ = CalcUnitStat();
  xtat_ = CalcHpMp();
}

void Hero::PutOn(const Equipment* equipment) {
  equipment_set_->SetEquipment(equipment);
}

HpMp Hero::CalcHpMp() const {
  HpMp xtat;
#define UPDATE_HPMP(x, xc) xtat.x = GetClass()->GetBni##xc().base + \
                                    GetClass()->GetBni##xc().incr * level_.level
  UPDATE_HPMP(hp, Hp);
  UPDATE_HPMP(mp, Mp);
#undef UPDATE_HPMP
  return xtat;
}

Attribute Hero::CalcUnitStat() const {
  Attribute unit_stat;
#define UPDATE_ABILITIES(x) unit_stat.x = ((hero_stat_.x / 2) + \
                                      ((100 + 10 * (GetClass()->GetStatGrade()->x - 1)) * level_.level * hero_stat_.x) / 2000)
  UPDATE_ABILITIES(atk);
  UPDATE_ABILITIES(def);
  UPDATE_ABILITIES(dex);
  UPDATE_ABILITIES(itl);
  UPDATE_ABILITIES(mor);
#undef UPDATE_ABILITIES
  return unit_stat;
}

void Hero::UpdateStat() {
  unit_stat_ = CalcUnitStat();
  { // FIXME code copied from Unit
    Attribute addends = equipment_set_->CalcAddends();
    Attribute multipliers = equipment_set_->CalcMultipliers();

    unit_stat_ += addends;
    unit_stat_ *= multipliers + 100;
    unit_stat_ /= 100;
  }
}

} // namespace core
} // namespace mengde

#include "unit.h"

#include "cmd.h"// TODO Only for CmdQueue
#include "unit_class.h"
#include "equipment_set.h"

namespace mengde {
namespace core {

Unit::Unit(const Hero* hero, Force force)
    : hero_(new Hero(*hero)), // NOTE deep-copy is done here
      equipment_set_(new EquipmentSet(this)),
      current_attr_(hero->GetUnitStat()),
      current_hpmp_(hero->GetHpMp()),
      modifier_list_(),
      effect_list_(),
      position_(0, 0),
      direction_(kDirDown),
      force_(force),
      no_render_(false),
      done_action_(false) {
  equipment_set_->CopyEquipmentSet(*hero_->GetEquipmentSet());
}

Unit::~Unit() {
  delete hero_;
  delete equipment_set_;
}

// Return true if alive, false if dead
bool Unit::DoDamage(int damage) {
  current_hpmp_.hp -= damage;
  return current_hpmp_.hp > 0;
}

void Unit::RestoreHP(int amount) {
  Heal(amount);
}

void Unit::Heal(int amount) {
  current_hpmp_.hp = std::min(current_hpmp_.hp + amount, GetOriginalHpMp().hp);
}

bool Unit::IsHostile(Unit* u) const {
  Force uforce = u->force_;
  if (((uint32_t)force_ & (uint32_t)Force::kFriendly) && ((uint32_t)uforce & (uint32_t)Force::kEnemy))
    return true;
  if (((uint32_t)uforce & (uint32_t)Force::kFriendly) && ((uint32_t)force_ & (uint32_t)Force::kEnemy))
    return true;
  return false;
}

std::string Unit::GetId() {
  return hero_->GetId();
}

uint16_t Unit::GetLevel() const {
  return hero_->GetLevel();
}

uint16_t Unit::GetExp() const {
  return hero_->GetExp();
}

int Unit::GetMove() {
  return hero_->GetMove();
}

std::string Unit::GetModelId() {
  return hero_->GetModelId();
}

const Attribute& Unit::GetOriginalStat() const {
  return hero_->GetUnitPureStat();
}

const HpMp& Unit::GetOriginalHpMp() const {
  return hero_->GetHpMp();
}

void Unit::UpdateStat() {
  current_attr_ = hero_->GetUnitPureStat();
  {
    Attribute addends = modifier_list_.CalcAddends() + equipment_set_->CalcAddends();
    Attribute multipliers = modifier_list_.CalcMultipliers() + equipment_set_->CalcMultipliers();

    current_attr_.ApplyModifier(addends, multipliers);
  }
}

void Unit::AddStatModifier(StatModifier* sm) {
  modifier_list_.AddModifier(sm);
  UpdateStat();
}

bool Unit::IsHPLow() const {
  return GetCurrentHpMp().hp <= GetOriginalHpMp().hp * 3 / 10;
}

bool Unit::IsDead() const {
  return GetCurrentHpMp().hp <= 0;
}

void Unit::Kill() {
  current_hpmp_.hp = 0;
}

const UnitClass* Unit::GetClass() const {
  return hero_->GetClass();
}

int Unit::GetClassIndex() const {
  return hero_->GetClassIndex();
}

Vec2D* Unit::GetAttackRange() const {
  return hero_->GetAttackRange();
}

bool Unit::IsInRange(Vec2D c, Vec2D* r) const {
  Vec2D dv = c - position_;
  Vec2D* ranges = r;
  while (*ranges != Vec2D(0, 0)) {
    if (*ranges == dv)
      return true;
    ranges++;
  }
  return false;
}

bool Unit::IsInRange(Vec2D c) const {
  return IsInRange(c, GetAttackRange());
}

void Unit::GainExp(Unit* object) {
  int level_diff = object->GetLevel() - this->GetLevel();
  uint16_t exp = 0;
  if (level_diff < 0) {
    exp = std::max(1, 8 + level_diff);
  } else {
    exp = std::min(100, 8 + 2 * level_diff);
  }
  GainExp(exp);
}

void Unit::GainExp(uint16_t exp) {
  hero_->GainExp(exp);
}

void Unit::LevelUp() {
  // TODO check if Unit is alread in max level
  hero_->LevelUp();
  current_attr_ = hero_->GetUnitStat();
  LOG_INFO("'%s' Level Up! (Level : %d)", hero_->GetId().c_str(), hero_->GetLevel());
}

void Unit::EndAction() {
  done_action_ = true;
}

void Unit::ResetAction() {
  done_action_ = false;
}

unique_ptr<Cmd> Unit::RaiseEvent(event::GeneralEvent type, Unit* unit) const {
  ASSERT(unit == this);
  return equipment_set_->RaiseEvent(type, unit);
}

void Unit::RaiseEvent(event::OnCmdEvent type, Unit* unit, CmdAct* act) const {
  ASSERT(unit == this);
  equipment_set_->RaiseEvent(type, unit, act);
}

unique_ptr<Cmd> Unit::RaiseEvent(event::GeneralEvent type) {
  return RaiseEvent(type, this);
}

void Unit::RaiseEvent(event::OnCmdEvent type, CmdAct* act) {
  RaiseEvent(type, this, act);
}

} // namespace core
} // namespace mengde

#include "unit.h"

#include "cmd.h"  // TODO Only for CmdQueue
#include "equipment_set.h"
#include "unit_class.h"

namespace mengde {
namespace core {

Unit::Unit(const Hero* hero, Force force)
    : hero_(new Hero(*hero)),  // NOTE deep-copy is done here
      equipment_set_(new EquipmentSet(this)),
      current_attr_(hero->GetOriginalAttr()),
      current_hpmp_(hero->GetOriginalHpMp()),
      modifier_list_(),
      effect_list_(),
      position_(0, 0),
      direction_(kDirDown),
      force_(force),
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

void Unit::RestoreHP(int amount) { Heal(amount); }

void Unit::Heal(int amount) { current_hpmp_.hp = std::min(current_hpmp_.hp + amount, GetOriginalHpMp().hp); }

bool Unit::IsHostile(const Unit* u) const {
  Force uforce = u->force_;
  if (((uint32_t)force_ & (uint32_t)Force::kFriendly) && ((uint32_t)uforce & (uint32_t)Force::kEnemy)) return true;
  if (((uint32_t)uforce & (uint32_t)Force::kFriendly) && ((uint32_t)force_ & (uint32_t)Force::kEnemy)) return true;
  return false;
}

string Unit::id() const { return hero_->id(); }

uint16_t Unit::GetLevel() const { return hero_->GetLevel(); }

uint16_t Unit::GetExp() const { return hero_->GetExp(); }

int Unit::move() const { return hero_->move(); }

const Attribute& Unit::GetOriginalAttr() const { return hero_->GetUnitPureStat(); }

const HpMp& Unit::GetOriginalHpMp() const { return hero_->GetOriginalHpMp(); }

void Unit::UpdateStat() {
  // TODO update HpMp
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

bool Unit::IsHPLow() const { return GetCurrentHpMp().hp <= GetOriginalHpMp().hp * 3 / 10; }

bool Unit::IsDead() const { return GetCurrentHpMp().hp <= 0; }

void Unit::Kill() { current_hpmp_.hp = 0; }

const UnitClass* Unit::unit_class() const { return hero_->unit_class(); }

int Unit::class_index() const { return hero_->class_index(); }

const AttackRange& Unit::attack_range() const { return hero_->attack_range(); }

bool Unit::IsInRange(Vec2D c, const AttackRange& range) const {
  Vec2D dv = c - position_;
  bool res = false;
  range.ForEach([&](Vec2D d) {
    // TODO Minor Optimization : Break when found
    res |= (dv == d);
  });
  return res;
}

bool Unit::IsInRange(Vec2D c) const { return IsInRange(c, attack_range()); }

bool Unit::GainExp(uint16_t exp) {
  hero_->GainExp(exp);
  return hero_->IsExpFull();
}

void Unit::LevelUp() {
  // TODO check if Unit is alread in max level
  hero_->LevelUp();
  UpdateStat();
  LOG_INFO("'%s' Level Up! (Level : %d)", hero_->id().c_str(), hero_->GetLevel());
}

bool Unit::ReadyPromotion() const { return hero_->ReadyPromotion(); }

void Unit::Promote(const UnitClassManager* ucm) {
  hero_->Promote(ucm);
  UpdateStat();
}

void Unit::EndAction() { done_action_ = true; }

void Unit::ResetAction() { done_action_ = false; }

unique_ptr<Cmd> Unit::RaiseEvent(event::GeneralEvent type, Unit* unit) const {
  ASSERT(unit == this);
  return equipment_set_->RaiseEvent(type, unit);
}

void Unit::RaiseEvent(event::OnCmdEvent type, Unit* unit, CmdAct* act) const {
  ASSERT(unit == this);
  equipment_set_->RaiseEvent(type, unit, act);
}

unique_ptr<Cmd> Unit::RaiseEvent(event::GeneralEvent type) { return RaiseEvent(type, this); }

void Unit::RaiseEvent(event::OnCmdEvent type, CmdAct* act) { RaiseEvent(type, this, act); }

}  // namespace core
}  // namespace mengde

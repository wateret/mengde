#include "unit.h"
#include "unit_class.h"

Unit::Unit(shared_ptr<Hero> hero, const Force force)
    : hero_(hero),
      equipment_slot_weapon_(Equipment::Type::kWeapon),
      equipment_slot_armor_(Equipment::Type::kArmor),
      equipment_slot_aid_(Equipment::Type::kAid),
      current_stat_(hero->GetUnitStat()),
      current_xtat_(hero->GetXtat()),
      position_(0, 0),
      direction_(kDirDown),
      stat_modifier_list_(),
      force_(force),
      no_render_(false),
      done_action_(false) {
}

Unit::~Unit() {
}

// Return true if alive, false if dead
bool Unit::DoDamage(int damage) {
  current_xtat_.hp -= damage;
  return current_xtat_.hp > 0;
}

void Unit::RestoreHP(int amount) {
  Heal(amount);
}

void Unit::Heal(int amount) {
  current_xtat_.hp = std::min(current_xtat_.hp + amount, GetOriginalXtat().hp);
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

int Unit::GetLevel() {
  return hero_->GetLevel();
}

int Unit::GetMove() {
  return hero_->GetMove();
}

std::string Unit::GetBitmapPath() {
  return hero_->GetBitmapPath();
}

const Stat& Unit::GetOriginalStat() const {
  return hero_->GetUnitStat();
}

const Xtat& Unit::GetOriginalXtat() const {
  return hero_->GetXtat();
}

void Unit::RecalcStat() {
  current_stat_ = hero_->GetUnitStat();

  Stat addends = stat_modifier_list_.CalcAddends() +
                 equipment_slot_weapon_.CalcModifierAddends() +
                 equipment_slot_armor_.CalcModifierAddends() +
                 equipment_slot_aid_.CalcModifierAddends();
  Stat multipliers = stat_modifier_list_.CalcMultipliers() +
                     equipment_slot_weapon_.CalcModifierMultipliers() +
                     equipment_slot_armor_.CalcModifierMultipliers() +
                     equipment_slot_aid_.CalcModifierMultipliers();

  for (uint32_t i = 0; i < NUM_STATS; i++) {
    current_stat_.AddValueByIndex(i, addends.GetValueByIndex(i));
    int val = current_stat_.GetValueByIndex(i);
    int val_mult = ((100 + multipliers.GetValueByIndex(i)) * val) / 100;
    current_stat_.SetValueByIndex(i, val_mult);
  }
}

void Unit::AddStatModifier(StatModifier* sm) {
  stat_modifier_list_.AddModifier(sm);
  RecalcStat();
}

bool Unit::IsHPLow() const {
  return GetCurrentXtat().hp <= GetOriginalXtat().hp * 3 / 10;
}

bool Unit::IsDead() const {
  return GetCurrentXtat().hp <= 0;
}

void Unit::Kill() {
  current_xtat_.hp = 0;
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
  int exp = 0;
  if (level_diff < 0) {
    exp = std::max(1, 8 + level_diff);
  } else {
    exp = std::min(100, 8 + 2 * level_diff);
  }
  GainExp(exp);
}

void Unit::GainExp(int exp) {
  // TODO check if Unit is alread in max level
  current_xtat_.exp += exp;
  while (current_xtat_.exp >= GetMaxExp()) {
    current_xtat_.exp -= GetMaxExp();
    LevelUp();
  }
}

void Unit::LevelUp() {
  // TODO check if Unit is alread in max level
  hero_->LevelUp();
  current_stat_ = hero_->GetUnitStat();
  LOG_INFO("'%s' Level Up! (Level : %d)", hero_->GetId().c_str(), hero_->GetLevel());
}

void Unit::PutWeaponOn(Equipment* equipment) {
  equipment_slot_weapon_.PutEquipmentOn(equipment);
  RecalcStat();
}

void Unit::PutArmorOn(Equipment* equipment) {
  equipment_slot_armor_.PutEquipmentOn(equipment);
  RecalcStat();
}

void Unit::PutAidOn(Equipment* equipment) {
  equipment_slot_aid_.PutEquipmentOn(equipment);
  RecalcStat();
}

Equipment* Unit::GetWeapon() {
  return equipment_slot_weapon_.GetEquipment();
}

Equipment* Unit::GetArmor() {
  return equipment_slot_armor_.GetEquipment();
}

Equipment* Unit::GetAid() {
  return equipment_slot_aid_.GetEquipment();
}

void Unit::EndAction() {
  RaiseEvent(EventEffect::Type::kOnActionDone);
  done_action_ = true;
}

void Unit::ResetAction() {
  done_action_ = false;
}

void Unit::RaiseEvent(EventEffect::Type type, Unit* unit) {
  ASSERT(unit == this);

  Equipment* weapon = GetWeapon();
  Equipment* armor = GetArmor();
  Equipment* aid = GetAid();
  if (weapon != nullptr) weapon->RaiseEvent(type, unit);
  if (armor != nullptr) armor->RaiseEvent(type, unit);
  if (aid != nullptr) aid->RaiseEvent(type, unit);
}

void Unit::RaiseEvent(EventEffect::Type type) {
  RaiseEvent(type, this);
}


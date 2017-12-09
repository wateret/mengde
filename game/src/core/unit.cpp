#include "unit.h"
#include "unit_class.h"

Unit::Unit(Hero* hero, const Side side)
    : hero_(hero),
      item_slot_weapon_(Item::kItemWeapon),
      item_slot_armor_(Item::kItemArmor),
      item_slot_aid_(Item::kItemAid),
      current_stat_(*hero->GetUnitStat()),
      current_xtat_(*hero->GetXtat()),
      position_(0, 0),
      direction_(kDirDown),
      stat_modifier_list_(),
      side_(side),
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
  current_xtat_.hp += amount;
  if (current_xtat_.hp > GetOriginalXtat()->hp) {
    current_xtat_.hp = GetOriginalXtat()->hp;
  }
}

bool Unit::IsHostile(Unit* u) const {
  Side uside = u->side_;
  if ((side_ & kSideFriendly) && (uside & kSideEnemy))
    return true;
  if ((uside & kSideFriendly) && (side_ & kSideEnemy))
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

const Stat* Unit::GetOriginalStat() const {
  return hero_->GetUnitStat();
}

const Xtat* Unit::GetOriginalXtat() const {
  return hero_->GetXtat();
}

void Unit::RecalcStat() {
  current_stat_ = *hero_->GetUnitStat();

  Stat addends = stat_modifier_list_.CalcAddends() +
                 item_slot_weapon_.CalcModifierAddends() +
                 item_slot_armor_.CalcModifierAddends() +
                 item_slot_aid_.CalcModifierAddends();
  Stat multipliers = stat_modifier_list_.CalcMultipliers() +
                     item_slot_weapon_.CalcModifierMultipliers() +
                     item_slot_armor_.CalcModifierMultipliers() +
                     item_slot_aid_.CalcModifierMultipliers();

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
  return GetCurrentXtat()->hp <= GetOriginalXtat()->hp * 3 / 10;
}

bool Unit::IsHPZero() const {
  return GetCurrentXtat()->hp <= 0;
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
  current_stat_ = *hero_->GetUnitStat();
  LOG_INFO("'%s' Level Up! (Level : %d)", hero_->GetId().c_str(), hero_->GetLevel());
}

void Unit::PutWeaponOn(Item* item) {
  item_slot_weapon_.PutItemOn(item);
  RecalcStat();
}

void Unit::PutArmorOn(Item* item) {
  item_slot_armor_.PutItemOn(item);
  RecalcStat();
}

void Unit::PutAidOn(Item* item) {
  item_slot_aid_.PutItemOn(item);
  RecalcStat();
}

Item* Unit::GetWeapon() {
  return item_slot_weapon_.GetItem();
}

Item* Unit::GetArmor() {
  return item_slot_armor_.GetItem();
}

Item* Unit::GetAid() {
  return item_slot_aid_.GetItem();
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

  Item* weapon = GetWeapon();
  Item* armor = GetArmor();
  Item* aid = GetAid();
  if (weapon != nullptr) weapon->RaiseEvent(type, this);
  if (armor != nullptr) armor->RaiseEvent(type, this);
  if (aid != nullptr) aid->RaiseEvent(type, this);
}

void Unit::RaiseEvent(EventEffect::Type type) {
  RaiseEvent(type, this);
}


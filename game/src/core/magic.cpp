#include "magic.h"
#include "unit.h"
#include "stat_modifier_list.h"
#include "stat_modifier.h"
#include "formulae.h"

Magic::Magic(const std::string& id,
             MagicType type,
             Range::Type range,
             bool is_target_enemy,
             uint16_t mp_cost,
             uint16_t power,
             uint16_t stat_id,
             uint16_t amount,
             uint16_t turns)
    : id_(id),
      type_(type),
      range_(range),
      learn_info_list_(),
      is_target_enemy_(is_target_enemy),
      mp_cost_(mp_cost),
      power_(power),
      stat_id_(stat_id),
      amount_(amount),
      turns_(turns) {
}

void Magic::Perform(Unit* unit_atk, Unit* unit_def) {
  /*
           */
//  if (TryPerform(unit_atk, unit_def)) {
  LOG_INFO("Magic hits!");

  if (type_ & kMagicDeal) {
    int damage = CalcDamage(unit_atk, unit_def);
    unit_def->DoDamage(damage);
    LOG_INFO("Magic does damage by %d", damage);
  }

  if (type_ & kMagicHeal) {
    int amount = CalcDamage(unit_atk, unit_def); // FIXME change formula
    unit_def->Heal(amount);
    LOG_INFO("Magic heals HP by %d", amount);
  }

  if (type_ & kMagicStatMod) {
    unit_def->AddStatModifier(new StatModifier("magic", stat_id_, 0, amount_, turns_));
  }
}

int Magic::CalcDamage(Unit* unit_atk, Unit* unit_def) {
  return Formulae::ComputeMagicDamage(nullptr, unit_atk, unit_def, 100 /* force */);
}

int Magic::CalcAccuracy(Unit* unit_atk, Unit* unit_def) {
  return Formulae::ComputeMagicAccuracy(unit_atk, unit_def, 100 /* force */);
}

bool Magic::TryPerform(Unit* unit_atk, Unit* unit_def) {
  return GenRandom(100) < CalcAccuracy(unit_atk, unit_def);
}

void Magic::AddLearnInfo(uint16_t class_id, uint16_t level) {
  learn_info_list_.push_back({class_id, level});
}

bool Magic::IsAvailible(Unit* unit) {
  for (auto e : learn_info_list_) {
    if (unit->GetClassIndex() == e.id && unit->GetLevel() >= e.lv) {
      return true;
    }
  }
  return false;
}

Vec2D* Magic::GetRange() {
  return Range::kRanges[range_];
}


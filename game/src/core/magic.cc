#include "magic.h"

#include "attribute_modifier.h"
#include "attribute_modifier_list.h"
#include "formulae.h"
#include "unit.h"

namespace mengde {
namespace core {

//
// MagicEffect
//
MagicEffect::MagicEffect(MagicEffectType type) : type_{type} {}

//
// MagicEffectDeal
//
MagicEffectHP::MagicEffectHP(int32_t power) : MagicEffect{MagicEffectType::kHP}, power_{power} {}

int MagicEffectHP::Diff(const Unit* atk, const Unit* def) {
  if (power_ < 0) {
    return -formulae::ComputeMagicDamage(nullptr, atk, def, -power_);
  } else {
    return formulae::ComputeMagicDamage(nullptr, atk, def, power_);  // TODO Fix it with proper calculation
  }
}

void MagicEffectHP::Perform(Unit* atk, Unit* def) {
  auto diff = Diff(atk, def);
  if (power_ < 0) {
    def->DoDamage(-diff);
    LOG_INFO("Magic does damage by %d", -diff);
  } else {
    def->Heal(diff);
    LOG_INFO("Magic heals HP by %d", diff);
  }
}

//
// MagicEffectStat
//
MagicEffectStat::MagicEffectStat(const AttributeModifier& attribute_modifier)
    : MagicEffect{MagicEffectType::kStat}, attribute_modifier_{attribute_modifier} {}

void MagicEffectStat::Perform(Unit* atk, Unit* def) {
  UNUSED(atk);
  def->AddAttributeModifier(new AttributeModifier(attribute_modifier_));
  LOG_INFO("Magic changes stat id %u by {%d+, %d%}", attribute_modifier_.stat_id(), attribute_modifier_.addend(),
           attribute_modifier_.multiplier());
}

//
// MagicEffectCondition
//

MagicEffectCondition::MagicEffectCondition(Condition condition, TurnBased turns)
    : MagicEffect{MagicEffectType::kCondition}, condition_{condition}, turns_{turns} {}

void MagicEffectCondition::Perform(Unit* atk, Unit* def) {
  UNUSED(atk);
  def->condition_set().Set(condition_, turns_);
  LOG_INFO("Magic changes condition");
}

Magic::Magic(const std::string& id, Range::Type range, bool is_target_enemy, uint16_t mp_cost)
    : id_(id), range_(range), learn_info_list_(), is_target_enemy_(is_target_enemy), mp_cost_(mp_cost) {
  // TODO Make use of these
  UNUSED(mp_cost_);
}

void Magic::Perform(Unit* unit_atk, Unit* unit_def) {
  for (auto&& effect : effects_) {
    effect.second->Perform(unit_atk, unit_def);
  }
}

int Magic::CalcAccuracy(const Unit* unit_atk, const Unit* unit_def) const {
  return formulae::ComputeMagicAccuracy(unit_atk, unit_def, 100 /* force */);
}

bool Magic::TryPerform(Unit* unit_atk, Unit* unit_def) { return GenRandom(100) < CalcAccuracy(unit_atk, unit_def); }

void Magic::AddLearnInfo(uint16_t class_id, uint16_t level) { learn_info_list_.push_back({class_id, level}); }

void Magic::AddEffect(std::unique_ptr<MagicEffect>&& effect) {
  effects_.insert(std::make_pair(effect->type(), std::move(effect)));
}

bool Magic::IsAvailible(const Unit* unit) const {
  for (auto e : learn_info_list_) {
    if (unit->class_index() == e.id && unit->GetLevel() >= e.lv) {
      return true;
    }
  }
  return false;
}

const AttackRange& Magic::GetRange() const { return AttackRangeManager::GetInstance().Get(range_); }

bool Magic::HasHP() const { return effects_.find(MagicEffectType::kHP) != effects_.end(); }

int Magic::HPDiff(const Unit* atk, const Unit* def) const {
  auto effect = effects_.find(MagicEffectType::kHP);
  if (effect == effects_.end()) return 0;
  auto effect_hp = dynamic_cast<MagicEffectHP*>(effect->second.get());
  return (effect_hp != nullptr) ? effect_hp->Diff(atk, def) : 0;
}

}  // namespace core
}  // namespace mengde

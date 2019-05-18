#ifndef MENGDE_CORE_MAGIC_H_
#define MENGDE_CORE_MAGIC_H_

#include <string>
#include <unordered_map>

#include "attack_range.h"
#include "attribute_modifier.h"
#include "condition.h"
#include "turn_based.h"

namespace mengde {
namespace core {

class Unit;

enum class MagicEffectType {
  kNone = 0x00,       //< Unused
  kHP = 0x01,         //< Do damage or heal
  kStat = 0x02,       //< Stat modification
  kCondition = 0x04,  //< Release Unit Condition
};

}  // namespace core
}  // namespace mengde

#include <functional>
#include <type_traits>

namespace std {
template <>
struct hash<::mengde::core::MagicEffectType> {
  size_t operator()(::mengde::core::MagicEffectType arg) const {
    using type = std::underlying_type<::mengde::core::MagicEffectType>::type;
    return std::hash<type>{}(static_cast<type>(arg));
  }
};
}  // namespace std

namespace mengde {
namespace core {

class MagicEffect {
 public:
  MagicEffect(MagicEffectType type);
  MagicEffectType type() const { return type_; }
  virtual ~MagicEffect() = default;
  virtual void Perform(Unit* atk, Unit* def) = 0;

 private:
  MagicEffectType type_;
};

class MagicEffectHP : public MagicEffect {
 public:
  MagicEffectHP(int32_t power);
  int Diff(const Unit* atk, const Unit* def);
  int32_t power() const { return power_; }

 public:
  virtual void Perform(Unit* atk, Unit* def) override;

 private:
  int32_t power_;
};

class MagicEffectStat : public MagicEffect {
 public:
  MagicEffectStat(const AttributeModifier& attribute_modifier);
  const AttributeModifier& attribute_modifier() const { return attribute_modifier_; }

 public:
  virtual void Perform(Unit* atk, Unit* def) override;

 private:
  AttributeModifier attribute_modifier_;
};

class MagicEffectCondition : public MagicEffect {
 public:
  MagicEffectCondition(Condition condition, TurnBased turns);
  const Condition& condition() const { return condition_; }
  const TurnBased& turn() const { return turns_; }

 public:
  virtual void Perform(Unit* atk, Unit* def) override;

 private:
  Condition condition_;
  TurnBased turns_;
};

class Magic {
 public:
  struct LearnInfo {
    uint16_t id;
    uint16_t lv;
    LearnInfo(uint16_t id, uint16_t lv) : id(id), lv(lv) {}
  };

 public:
  Magic(const std::string&, Range::Type, bool target, uint16_t mp);
  const string& GetId() const { return id_; }
  bool is_target_enemy() const { return is_target_enemy_; }
  void Perform(Unit*, Unit*);
  void AddLearnInfo(uint16_t, uint16_t);
  void AddEffect(std::unique_ptr<MagicEffect>&& effect);
  const AttackRange& GetRange() const;

 public:
  int CalcAccuracy(const Unit*, const Unit*) const;
  bool TryPerform(Unit*, Unit*);
  bool IsAvailible(const Unit*) const;
  bool HasHP() const;
  int HPDiff(const Unit* atk, const Unit* def) const;
  Range::Type range_enum() const { return range_; }
  uint16_t mp_cost() const { return mp_cost_; }
  const vector<LearnInfo>& learn_info_list() const { return learn_info_list_; }
  const std::unordered_map<MagicEffectType, std::unique_ptr<MagicEffect>>& effects() const { return effects_; }

 private:
  string id_;
  Range::Type range_;
  vector<LearnInfo> learn_info_list_;
  bool is_target_enemy_;
  uint16_t mp_cost_;
  std::unordered_map<MagicEffectType, std::unique_ptr<MagicEffect>> effects_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_MAGIC_H_

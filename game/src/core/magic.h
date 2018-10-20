#ifndef MENGDE_CORE_MAGIC_H_
#define MENGDE_CORE_MAGIC_H_

#include <string>
#include <unordered_map>

#include "attack_range.h"
#include "condition.h"
#include "stat_modifier.h"
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

 public:
  virtual void Perform(Unit* atk, Unit* def) override;

 private:
  int32_t power_;
};

class MagicEffectStat : public MagicEffect {
 public:
  MagicEffectStat(uint16_t stat_id, StatMod stat_mod, TurnBased turns);

 public:
  virtual void Perform(Unit* atk, Unit* def) override;

 private:
  uint16_t stat_id_;
  StatMod stat_mod_;
  TurnBased turns_;
};

class MagicEffectCondition : public MagicEffect {
 public:
  MagicEffectCondition(Condition condition, TurnBased turns);

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
  bool GetIsTargetEnemy() { return is_target_enemy_; }
  void Perform(Unit*, Unit*);
  void AddLearnInfo(uint16_t, uint16_t);
  void AddEffect(std::unique_ptr<MagicEffect>&& effect);
  const AttackRange& GetRange() const;

 public:
  int CalcDamage(Unit*, Unit*);
  int CalcAccuracy(Unit*, Unit*);
  bool TryPerform(Unit*, Unit*);
  bool IsAvailible(const Unit*);
  bool IsGood() const;

 private:
  string id_;
  MagicEffectType type_;
  Range::Type range_;
  vector<LearnInfo> learn_info_list_;
  bool is_target_enemy_;
  uint16_t mp_cost_;
  std::unordered_map<MagicEffectType, std::unique_ptr<MagicEffect>> effects_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_MAGIC_H_

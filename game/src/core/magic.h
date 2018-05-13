#ifndef MENGDE_CORE_MAGIC_H_
#define MENGDE_CORE_MAGIC_H_

#include <string>

#include "attack_range.h"

namespace mengde {
namespace core {

class Unit;

class Magic {
 public:
  enum MagicType {
    kMagicNone = 0x00,
    kMagicDeal = 0x01,
    kMagicHeal = 0x02,
    kMagicStatMod = 0x04,
  };

  struct LearnInfo {
    uint16_t id;
    uint16_t lv;
    LearnInfo(uint16_t id, uint16_t lv) : id(id), lv(lv) {}
  };

 public:
  Magic(const std::string&, MagicType, Range::Type, bool, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
  const string& GetId() const { return id_; }
  bool GetIsTargetEnemy() { return is_target_enemy_; }
  void Perform(Unit*, Unit*);
  void AddLearnInfo(uint16_t, uint16_t);

  const AttackRange& GetRange();

 public:
  int CalcDamage(Unit*, Unit*);
  int CalcAccuracy(Unit*, Unit*);
  bool TryPerform(Unit*, Unit*);
  bool IsAvailible(const Unit*);
  bool IsTypeHeal() { return type_ & kMagicHeal; }
  bool IsTypeDeal() { return type_ & kMagicDeal; }

 private:
  string id_;
  MagicType type_;
  Range::Type range_;
  vector<LearnInfo> learn_info_list_;
  bool is_target_enemy_;
  uint16_t mp_cost_;
  uint16_t power_;
  uint16_t stat_id_;
  uint16_t amount_;
  uint16_t turns_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_MAGIC_H_

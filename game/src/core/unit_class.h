#ifndef MENGDE_CORE_UNIT_CLASS_H_
#define MENGDE_CORE_UNIT_CLASS_H_

#include <boost/optional.hpp>

#include "attack_range.h"
#include "stat.h"
#include "util/common.h"

namespace mengde {
namespace core {

struct BaseAndIncr {
  int base;
  int incr;
  BaseAndIncr(int base, int incr) : base(base), incr(incr) {}
};

struct PromotionInfo {
  string id;
  int level;
  PromotionInfo(const string& id, int level) : id(id), level(level) {}
};

class UnitClass {
 public:
  UnitClass(const string&, const int, const Attribute&, const Range::Type, const int, const BaseAndIncr,
            const BaseAndIncr, const boost::optional<PromotionInfo>& promotion_info);
  int GetIndex() const { return index_; }
  string GetId() const { return id_; }
  const AttackRange& attack_range() const { return AttackRangeManager::GetInstance().Get(attack_range_); }
  int move() const { return move_; }
  BaseAndIncr GetBniHp() const { return bni_hp_; }
  BaseAndIncr GetBniMp() const { return bni_mp_; }
  const Attribute& GetStatGrade() const { return stat_grade_; }

 private:
  string id_;
  int index_;
  Attribute stat_grade_;
  Range::Type attack_range_;
  int move_;
  BaseAndIncr bni_hp_;
  BaseAndIncr bni_mp_;
  boost::optional<PromotionInfo> promotion_info_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_UNIT_CLASS_H_

#ifndef UNIT_CLASS_H_
#define UNIT_CLASS_H_

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

class UnitClass {
 public:
  UnitClass(const string&, const int, const int, const Attribute&, const Range::Type, const int, const BaseAndIncr,
            const BaseAndIncr);
  int              GetIndex() const { return index_; }
  string           GetId() const { return id_; }
  int              GetNumPromotions() const { return num_promotions_; }
  Vec2D*           GetAttackRange() const { return Range::kRanges[attack_range_]; }
  int              GetMove() const { return move_; }
  BaseAndIncr      GetBniHp() const { return bni_hp_; }
  BaseAndIncr      GetBniMp() const { return bni_mp_; }
  const Attribute& GetStatGrade() const { return stat_grade_; }

 private:
  string      id_;
  int         index_;
  int         num_promotions_;
  Attribute   stat_grade_;
  Range::Type attack_range_;
  int         move_;
  BaseAndIncr bni_hp_;
  BaseAndIncr bni_mp_;
};

}  // namespace core
}  // namespace mengde

#endif  // UNIT_CLASS_H_

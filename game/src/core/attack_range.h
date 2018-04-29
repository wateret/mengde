#ifndef MENGDE_CORE_RANGE_ATTACK_RANGE_H_
#define MENGDE_CORE_RANGE_ATTACK_RANGE_H_

#include <functional>
#include <initializer_list>

#include "util/common.h"

namespace mengde {
namespace core {
namespace Range {

enum Type {
#define MACRO_ATTACK_RANGE(name, ...) k##name,
#include "attack_range.h.inc"
#undef MACRO_ATTACK_RANGE
  kCount
};

extern Vec2D* kRanges[kCount];

Type StringToRange(const string&);

}  // namespace Range
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace core {

class AttackRange {
 public:
  AttackRange();
  AttackRange(std::initializer_list<Vec2D> range_list);
  void ForEach(const std::function<void(Vec2D)>& fn) const;
  void ForEach(const std::function<void(Vec2D)>& fn, Vec2D pivot) const;

 private:
  vector<Vec2D> range_list_;
};

class AttackRangeManager {
 public:
  static AttackRangeManager& GetInstance();

 public:
  const AttackRange& Get(Range::Type type);

 private:
  AttackRangeManager();

 private:
  AttackRange ranges_[Range::Type::kCount];
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_RANGE_ATTACK_RANGE_H_

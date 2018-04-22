#include "attack_range.h"

#include "util/common.h"

namespace mengde {
namespace core {
namespace Range {

#define MACRO_ATTACK_RANGE(name, ...) Vec2D kRange##name[] = {__VA_ARGS__, {0, 0}};
#include "attack_range.h.inc"
#undef MACRO_ATTACK_RANGE

Vec2D* kRanges[kCount] = {
#define MACRO_ATTACK_RANGE(name, ...) kRange##name,
#include "attack_range.h.inc"
#undef MACRO_ATTACK_RANGE
};

static const char* kToString[kCount] = {
#define MACRO_ATTACK_RANGE(name, ...) #name,
#include "attack_range.h.inc"
#undef MACRO_ATTACK_RANGE
};

Type StringToRange(const string& s) {
  for (int i = 0; i < Range::kCount; i++) {
    if (s == Range::kToString[i]) return static_cast<Range::Type>(i);
  }
  UNREACHABLE("Undefined Range value");
  return Range::kNone;
}

}  // namespace Range
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace core {

AttackRange::AttackRange() {}

AttackRange::AttackRange(std::initializer_list<Vec2D> range_list) : range_list_(range_list) {}

void AttackRange::ForEach(const std::function<void(Vec2D)>& fn) const {
  std::for_each(range_list_.begin(), range_list_.end(), fn);
}

void AttackRange::ForEach(const std::function<void(Vec2D)>& fn, Vec2D pivot) const {
  for (auto e : range_list_) {
    fn(pivot + e);
  }
}

AttackRangeManager::AttackRangeManager() {
  // Initialize built-in attack ranges
#define MACRO_ATTACK_RANGE(name, ...) ranges_[Range::Type::k##name] = AttackRange({__VA_ARGS__});
#include "attack_range.h.inc"
#undef MACRO_ATTACK_RANGE
}

AttackRangeManager& AttackRangeManager::GetInstance() {
  static AttackRangeManager instance;
  return instance;
}

const AttackRange& AttackRangeManager::Get(Range::Type type) { return ranges_[type]; }

}  // namespace core
}  // namespace mengde

#include "attack_range.h"

#include "util/common.h"

namespace mengde {
namespace core {

namespace Range {

#define MACRO_ATTACK_RANGE(name, ...) \
    Vec2D kRange##name[] = {__VA_ARGS__, {0, 0}};
#include "attack_range.inc.h"
#undef MACRO_ATTACK_RANGE

Vec2D* kRanges[kCount] = {
#define MACRO_ATTACK_RANGE(name, ...) \
    kRange##name,
#include "attack_range.inc.h"
#undef MACRO_ATTACK_RANGE
};

static const char* kToString[kCount] = {
#define MACRO_ATTACK_RANGE(name, ...) \
    #name,
#include "attack_range.inc.h"
#undef MACRO_ATTACK_RANGE
};

Type StringToRange(const string& s) {
  for (int i = 0; i < Range::kCount; i++) {
    if (s == Range::kToString[i]) return static_cast<Range::Type>(i);
  }
  UNREACHABLE("Undefined Range value");
  return Range::kNone;
}

} // namespace Range
} // namespace core
} // namespace mengde

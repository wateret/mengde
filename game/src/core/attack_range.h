#ifndef ATTACK_RANGE_H_
#define ATTACK_RANGE_H_

#include "util/common.h"

namespace mengde {
namespace core {
namespace Range {

enum Type {
#define MACRO_ATTACK_RANGE(name, ...) k##name,
#include "attack_range.h.inc"
  kCount
};

extern Vec2D* kRanges[kCount];

Type StringToRange(const string&);

}  // namespace Range
}  // namespace core
}  // namespace mengde

#endif  // ATTACK_RANGE_H_

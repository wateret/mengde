#ifndef ATTACK_RANGE_H_
#define ATTACK_RANGE_H_

#include "util/common.h"

namespace Range {

enum Type {
#define MACRO_ATTACK_RANGE(name, ...) \
    k##name,
#include "attack_range.inc.h"
  kCount
};

extern Vec2D* kRanges[kCount];

Type StringToRange(const string&);

} // namespace Range

#endif // ATTACK_RANGE_H_

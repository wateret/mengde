#include "unit_class.h"

namespace mengde {
namespace core {

UnitClass::UnitClass(const string& id,
                     const int index,
                     const int num_promotions,
                     const Attribute& stat_grade,
                     const Range::Type range,
                     const int move,
                     const BaseAndIncr bni_hp,
                     const BaseAndIncr bni_mp)
    : id_(id),
      index_(index),
      num_promotions_(num_promotions),
      stat_grade_(stat_grade),
      attack_range_(range),
      move_(move),
      bni_hp_(bni_hp),
      bni_mp_(bni_mp) {
}

} // namespace core
} // namespace mengde

#include "hero_class.h"

namespace mengde {
namespace core {

HeroClass::HeroClass(const string& id, const int index, const Attribute& stat_grade, const Range::Type range,
                     const int move, const BaseAndIncr bni_hp, const BaseAndIncr bni_mp,
                     const boost::optional<PromotionInfo>& promotion_info)
    : id_{id},
      index_{index},
      stat_grade_{stat_grade},
      attack_range_{range},
      move_{move},
      bni_hp_{bni_hp},
      bni_mp_{bni_mp},
      promotion_info_{promotion_info} {}

}  // namespace core
}  // namespace mengde

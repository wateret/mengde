#include "volatile_attribute.h"

namespace mengde {
namespace core {

void VolatileAttribute::NextTurn() {
  attribute_modifier_list_.NextTurn();
  event_effect_list_.NextTurn();
}

}  // namespace core
}  // namespace mengde

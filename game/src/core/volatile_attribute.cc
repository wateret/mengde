#include "volatile_attribute.h"

namespace mengde {
namespace core {

VolatileAttribute::VolatileAttribute(const AttributeModifierList& aml, const EventEffectList& eel)
    : attribute_modifier_list_{aml}, event_effect_list_{eel} {}

void VolatileAttribute::NextTurn() {
  attribute_modifier_list_.NextTurn();
  event_effect_list_.NextTurn();
}

}  // namespace core
}  // namespace mengde

#include "volatile_attribute.h"

namespace mengde {
namespace core {

VolatileAttribute::VolatileAttribute()
    : attribute_modifier_list_{new AttributeModifierList}, event_effect_list_{new EventEffectList} {}

VolatileAttribute::VolatileAttribute(unique_ptr<AttributeModifierList>&& aml, unique_ptr<EventEffectList>&& eel)
    : attribute_modifier_list_{std::move(aml)}, event_effect_list_{std::move(eel)} {}

void VolatileAttribute::NextTurn() {
  attribute_modifier_list_->NextTurn();
  event_effect_list_->NextTurn();
}

}  // namespace core
}  // namespace mengde

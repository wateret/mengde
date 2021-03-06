#ifndef MENGDE_CORE_VOLATILE_ATTRIBUTE_H_
#define MENGDE_CORE_VOLATILE_ATTRIBUTE_H_

#include "attribute_modifier_list.h"
#include "event_effect_list.h"

namespace mengde {
namespace core {

class VolatileAttribute {
 public:
  VolatileAttribute();
  VolatileAttribute(unique_ptr<AttributeModifierList>&& aml, unique_ptr<EventEffectList>&& eel);

 public:
  const AttributeModifierList& attribute_modifier_list() const { return *attribute_modifier_list_; }
  const EventEffectList& event_effect_list() const { return *event_effect_list_; }
  AttributeModifierList& attribute_modifier_list() { return *attribute_modifier_list_; }
  EventEffectList& event_effect_list() { return *event_effect_list_; }

 public:
  void NextTurn();

 private:
  unique_ptr<AttributeModifierList> attribute_modifier_list_;
  unique_ptr<EventEffectList> event_effect_list_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_VOLATILE_ATTRIBUTE_H_

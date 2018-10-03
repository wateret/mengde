#ifndef MENGDE_CORE_VOLATILE_ATTRIBUTE_H_
#define MENGDE_CORE_VOLATILE_ATTRIBUTE_H_

#include "event_effect_list.h"
#include "stat_modifier_list.h"

namespace mengde {
namespace core {

class VolatileAttribute {
 public:
  VolatileAttribute() = default;

 public:
  const StatModifierList& stat_modifier_list() const { return stat_modifier_list_; }
  const EventEffectList& event_effect_list() const { return event_effect_list_; }
  StatModifierList& stat_modifier_list() { return stat_modifier_list_; }
  EventEffectList& event_effect_list() { return event_effect_list_; }

 public:
  void NextTurn();

 private:
  StatModifierList stat_modifier_list_;
  EventEffectList event_effect_list_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_VOLATILE_ATTRIBUTE_H_

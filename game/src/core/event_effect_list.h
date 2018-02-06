#ifndef EVENT_EFFECT_LIST_H_
#define EVENT_EFFECT_LIST_H_

#include <vector>

#include "event_effect.h"
#include "i_event.h"

namespace mengde {
namespace core {

class EventEffectList : public IEvent {
 public:
  virtual void RaiseEvent(EventType, Unit*) override;

 public:
  EventEffectList();
  ~EventEffectList();
  void AddEffect(EventEffect*);
  void NextTurn();

 private:
  std::vector<EventEffect*> elements_;
};

} // namespace core
} // namespace mengde

#endif // EVENT_EFFECT_LIST_H_

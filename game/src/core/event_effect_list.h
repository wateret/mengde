#ifndef EVENT_EFFECT_LIST_H_
#define EVENT_EFFECT_LIST_H_

#include <vector>

#include "event_effect.h"
#include "i_event.h"

class EventEffectList : public IEvent {
 public:
  void RaiseEvent(EventEffect::Type, Unit*) override;

 public:
  EventEffectList();
  ~EventEffectList();
  void AddEffect(EventEffect*);
  void NextTurn();

 private:
  std::vector<EventEffect*> elements_;
};

#endif // EVENT_EFFECT_LIST_H_

#ifndef EVENT_EFFECT_LIST_H_
#define EVENT_EFFECT_LIST_H_

#include <vector>

#include "event_effect.h"

class EventEffectList {
 public:
  EventEffectList();
  ~EventEffectList();
  void AddEffect(EventEffect*);
  void RaiseEvent(EventEffect::EventEffectType, Unit*, void* = nullptr);
  void NextTurn();

 private:
  std::vector<EventEffect*> elements_;
};

#endif // EVENT_EFFECT_LIST_H_

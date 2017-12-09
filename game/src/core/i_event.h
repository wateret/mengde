#ifndef I_EVENT_H_
#define I_EVENT_H_

#include "event_effect.h"

class IEvent {
 public:
   virtual void RaiseEvent(EventEffect::EventEffectType, Unit*, void*) = 0;
};

#endif // I_EVENT_H_

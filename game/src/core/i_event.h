#ifndef I_EVENT_H_
#define I_EVENT_H_

#include "event_effect.h"

// IEvent is an interface for raising game events, i.e. for notifying observers

class IEvent {
 public:
   virtual void RaiseEvent(EventEffect::Type, Unit*, void*) = 0;
};

#endif // I_EVENT_H_

#ifndef EVENT_FETCHER_H_
#define EVENT_FETCHER_H_

#include "event.h"

class EventFetcher {
 public:
  EventFetcher() = default;
  bool Poll();
  EventType event_type() const { return event_type_; }
  const Event& event() const { return event_; }

 private:
  EventType event_type_;
  Event event_;
};

#endif // EVENT_FETCHER_H_

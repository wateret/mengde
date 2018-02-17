#ifndef EVENT_FETCHER_H_
#define EVENT_FETCHER_H_

#include "event.h"

namespace mengde {
namespace gui {
namespace foundation {

class EventFetcher {
 public:
  EventFetcher() = default;
  bool         Poll();
  EventType    event_type() const { return event_type_; }
  const Event& event() const { return event_; }

 private:
  EventType event_type_;
  Event     event_;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // EVENT_FETCHER_H_

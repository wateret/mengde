#ifndef MENGDE_GUI_FOUNDATION_EVENT_H_
#define MENGDE_GUI_FOUNDATION_EVENT_H_

#include "events.h"

namespace mengde {
namespace gui {
namespace foundation {

// EventType
//
// Only listed supported events for this framework
// One may add more events that are not listed here from `SDL_events.h`

enum class EventType {
#define MACRO_EVENT_TYPE(CAMEL_TYPE, SNAKE_TYPE) k##CAMEL_TYPE,
#include "event_type.h.inc"
#undef MACRO_EVENT_TYPE
  kCount
};

// Event
//
// Event data as union

union Event {
#define MACRO_EVENT_TYPE(CAMEL_TYPE, SNAKE_TYPE) CAMEL_TYPE##Event SNAKE_TYPE;
#include "event_type.h.inc"
#undef MACRO_EVENT_TYPE

  Event() {}
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_FOUNDATION_EVENT_H_

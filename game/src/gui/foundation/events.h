#ifndef EVENTS_H_
#define EVENTS_H_

#include "mouse_button_event.h"
#include "mouse_motion_event.h"
#include "mouse_wheel_event.h"

namespace mengde {
namespace gui {
namespace foundation {

class UnsupportedEvent {
 public:
  UnsupportedEvent() = default;
};

class QuitEvent {
 public:
  QuitEvent() = default;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // EVENTS_H_

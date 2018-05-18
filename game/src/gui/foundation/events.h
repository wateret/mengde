#ifndef MENGDE_GUI_FOUNDATION_EVENTS_H_
#define MENGDE_GUI_FOUNDATION_EVENTS_H_

#include "mouse_button_event.h"
#include "mouse_motion_event.h"
#include "mouse_wheel_event.h"
#include "key_event.h"

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

#endif  // MENGDE_GUI_FOUNDATION_EVENTS_H_

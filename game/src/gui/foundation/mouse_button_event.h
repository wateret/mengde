#ifndef MENGDE_GUI_FOUNDATION_MOUSE_BUTTON_EVENT_H_
#define MENGDE_GUI_FOUNDATION_MOUSE_BUTTON_EVENT_H_

#include "util/common.h"

namespace mengde {
namespace gui {
namespace foundation {

class MouseButtonEvent {
 public:
  enum class Button : uint16_t { kNone, kLeft, kMiddle, kRight };

  enum class State : uint16_t { kIdle, kDown, kUp };

 public:
  MouseButtonEvent(const Button, const State, const Vec2D);
  bool   IsLeftButtonDown() const;
  bool   IsLeftButtonUp() const;
  bool   IsMiddleButtonDown() const;
  bool   IsMiddleButtonUp() const;
  bool   IsRightButtonDown() const;
  bool   IsRightButtonUp() const;
  Button GetButton() const { return mouse_button_; }
  State  GetState() const { return mouse_button_state_; }
  Vec2D  GetCoords() const { return coords_; }
  void   SetCoords(Vec2D coords) { coords_ = coords; }

 private:
  Button mouse_button_;
  State  mouse_button_state_;
  Vec2D  coords_;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_FOUNDATION_MOUSE_BUTTON_EVENT_H_

#include "mouse_button_event.h"

namespace mengde {
namespace gui {
namespace foundation {

MouseButtonEvent::MouseButtonEvent(const Button mouse_button,
                                   const State mouse_button_state,
                                   const Vec2D coords)
    : mouse_button_(mouse_button),
      mouse_button_state_(mouse_button_state),
      coords_(coords) {
}

bool MouseButtonEvent::IsLeftButtonDown() const {
  return mouse_button_ == Button::kLeft && mouse_button_state_ == State::kDown;
}

bool MouseButtonEvent::IsLeftButtonUp() const {
  return mouse_button_ == Button::kLeft && mouse_button_state_ == State::kUp;
}

bool MouseButtonEvent::IsMiddleButtonDown() const {
  return mouse_button_ == Button::kMiddle && mouse_button_state_ == State::kDown;
}

bool MouseButtonEvent::IsMiddleButtonUp() const {
  return mouse_button_ == Button::kMiddle && mouse_button_state_ == State::kUp;
}

bool MouseButtonEvent::IsRightButtonDown() const {
  return mouse_button_ == Button::kRight && mouse_button_state_ == State::kDown;
}

bool MouseButtonEvent::IsRightButtonUp() const {
  return mouse_button_ == Button::kRight && mouse_button_state_ == State::kUp;
}

} // namespace foundation
} // namespace gui
} // namespace mengde


#include "mouse_button_event.h"

MouseButtonEvent::MouseButtonEvent(const Button mouse_button,
                                   const State mouse_button_state,
                                   const Vec2D coords)
    : mouse_button_(mouse_button),
      mouse_button_state_(mouse_button_state),
      coords_(coords) {
}

bool MouseButtonEvent::IsLeftButtonDown() const {
  return mouse_button_ == kButtonLeft && mouse_button_state_ == kStateDown;
}

bool MouseButtonEvent::IsLeftButtonUp() const {
  return mouse_button_ == kButtonLeft && mouse_button_state_ == kStateUp;
}

bool MouseButtonEvent::IsMiddleButtonDown() const {
  return mouse_button_ == kButtonMiddle && mouse_button_state_ == kStateDown;
}

bool MouseButtonEvent::IsMiddleButtonUp() const {
  return mouse_button_ == kButtonMiddle && mouse_button_state_ == kStateUp;
}

bool MouseButtonEvent::IsRightButtonDown() const {
  return mouse_button_ == kButtonRight && mouse_button_state_ == kStateDown;
}

bool MouseButtonEvent::IsRightButtonUp() const {
  return mouse_button_ == kButtonRight && mouse_button_state_ == kStateUp;
}


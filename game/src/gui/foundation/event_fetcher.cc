#include "event_fetcher.h"

#include <SDL.h>

namespace mengde {
namespace gui {
namespace foundation {

bool EventFetcher::Poll() {
  SDL_Event e;
  bool has_pending_event = SDL_PollEvent(&e);
  switch (e.type) {
    case SDL_QUIT: {
      event_type_ = EventType::kQuit;
      break;
    }
    case SDL_MOUSEMOTION: {
      event_type_ = EventType::kMouseMotion;
      Vec2D coords = {e.motion.x, e.motion.y};
      Vec2D coords_rel = {e.motion.xrel, e.motion.yrel};
      event_.mouse_motion = MouseMotionEvent(MouseMotionEvent::Type::kOver, coords, coords_rel);
      break;
    }
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN: {
      event_type_ = EventType::kMouseButton;
      MouseButtonEvent::State state =
          (e.type == SDL_MOUSEBUTTONUP) ? MouseButtonEvent::State::kUp : MouseButtonEvent::State::kDown;
      MouseButtonEvent::Button button = MouseButtonEvent::Button::kNone;
      switch (e.button.button) {
        case SDL_BUTTON_LEFT:
          button = MouseButtonEvent::Button::kLeft;
          break;
        case SDL_BUTTON_MIDDLE:
          button = MouseButtonEvent::Button::kMiddle;
          break;
        case SDL_BUTTON_RIGHT:
          button = MouseButtonEvent::Button::kRight;
          break;
        default:
          UNREACHABLE("Unknown mouse button");
          break;
      }
      event_.mouse_button = MouseButtonEvent(button, state, Vec2D(e.button.x, e.motion.y));
      break;
    }
    case SDL_MOUSEWHEEL: {
      event_type_ = EventType::kMouseWheel;
      int32_t x, y;
      SDL_GetMouseState(&x, &y);
      MouseWheelEvent::Horizontal hor = MouseWheelEvent::Horizontal::kNone;
      if (e.wheel.x < 0) hor = MouseWheelEvent::Horizontal::kLeft;
      if (e.wheel.x > 0) hor = MouseWheelEvent::Horizontal::kRight;
      MouseWheelEvent::Vertical ver = MouseWheelEvent::Vertical::kNone;
      if (e.wheel.y < 0) ver = MouseWheelEvent::Vertical::kDown;  // TODO check the direction
      if (e.wheel.y > 0) ver = MouseWheelEvent::Vertical::kUp;
      event_.mouse_wheel = MouseWheelEvent(hor, ver, Vec2D(x, y));
      break;
    }
    case SDL_KEYUP:
    case SDL_KEYDOWN: {
      event_type_ = EventType::kKey;
      KeyEvent::State state = (e.type == SDL_KEYUP) ? KeyEvent::State::kUp : KeyEvent::State::kDown;
      KeyEvent::Mod mod = ((e.key.keysym.mod & KMOD_CTRL) ? KeyEvent::Mod::kCtrl : KeyEvent::Mod::kNone) |
                          ((e.key.keysym.mod & KMOD_SHIFT) ? KeyEvent::Mod::kShift : KeyEvent::Mod::kNone) |
                          ((e.key.keysym.mod & KMOD_ALT) ? KeyEvent::Mod::kAlt : KeyEvent::Mod::kNone);
      event_.key = KeyEvent(e.key.keysym.sym, mod, state, e.key.repeat, e.key.timestamp);
      LOG_DEBUG("KeyEvent: 0x%02x 0x%02x 0x%02x %u %u", e.key.keysym.sym, static_cast<uint16_t>(mod),
                static_cast<uint16_t>(state), e.key.repeat, e.key.timestamp);
      break;
    }
    default:
      event_type_ = EventType::kUnsupported;
      break;
  }
  return has_pending_event;
}

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

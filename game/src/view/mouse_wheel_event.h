#ifndef MOUSE_WHEEL_EVENT_H_
#define MOUSE_WHEEL_EVENT_H_

#include "util/common.h"

class MouseWheelEvent {
 public:
   enum class Horizontal : uint16_t {
     kNone,
     kLeft,
     kRight
   };

   enum class Vertical : uint16_t {
     kNone,
     kUp,
     kDown
   };

 public:
  MouseWheelEvent(Horizontal, Vertical, Vec2D);
  bool IsLeft() { return hor_ == Horizontal::kLeft; }
  bool IsRight() { return hor_ == Horizontal::kRight; }
  bool IsUp() { return ver_ == Vertical::kUp; }
  bool IsDown() { return ver_ == Vertical::kDown; }
  Vec2D GetCoords() { return coords_; }

 private:
  Horizontal hor_;
  Vertical   ver_;
  Vec2D      coords_;
};

#endif // MOUSE_WHEEL_EVENT_H_

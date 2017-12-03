#ifndef MOUSE_MOTION_EVENT_H_
#define MOUSE_MOTION_EVENT_H_

#include "util/common.h"

class MouseMotionEvent {
 public:
  enum Motion : uint16_t {
    kMotionNone,
    kMotionOver,
    kMotionOut
  };

  MouseMotionEvent(const MouseMotionEvent&, Vec2D);
  MouseMotionEvent(Motion = kMotionNone, const Vec2D = {0, 0}, const Vec2D = {0, 0});
  bool IsMotionOver() const { return motion_ == kMotionOver; }
  bool IsMotionOut() const { return motion_ == kMotionOut; }
  Motion GetMotion() const { return motion_; }
  Vec2D GetCoords() const { return coords_; }
  Vec2D GetCoordsRel() const { return coords_rel_; }
  Vec2D GetLastCoords() const { return coords_ - coords_rel_; }

 private:
  Motion motion_;
  Vec2D coords_;
  Vec2D coords_rel_;
};

#endif // MOUSE_MOTION_EVENT_H_

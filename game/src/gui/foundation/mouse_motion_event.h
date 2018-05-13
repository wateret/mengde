#ifndef MENGDE_GUI_FOUNDATION_MOUSE_MOTION_EVENT_H_
#define MENGDE_GUI_FOUNDATION_MOUSE_MOTION_EVENT_H_

#include "util/common.h"

namespace mengde {
namespace gui {
namespace foundation {

class MouseMotionEvent {
 public:
  enum class Type : uint16_t { kNone, kOver, kOut };

  MouseMotionEvent(const MouseMotionEvent&, Vec2D);
  MouseMotionEvent(Type, const Vec2D, const Vec2D);
  MouseMotionEvent(Type);
  bool IsMotionOver() const { return motion_ == Type::kOver; }
  bool IsMotionOut() const { return motion_ == Type::kOut; }
  Type GetMotion() const { return motion_; }
  Vec2D GetCoords() const { return coords_; }
  Vec2D GetCoordsRel() const { return coords_rel_; }
  Vec2D GetLastCoords() const { return coords_ - coords_rel_; }

 private:
  Type motion_;
  Vec2D coords_;
  Vec2D coords_rel_;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_FOUNDATION_MOUSE_MOTION_EVENT_H_

#include "mouse_motion_event.h"

MouseMotionEvent::MouseMotionEvent(const MouseMotionEvent& e, Vec2D change)
    : motion_(e.motion_), coords_(e.coords_ + change), coords_rel_(e.coords_rel_) {
}

MouseMotionEvent::MouseMotionEvent(Motion motion,
                                   const Vec2D coords,
                                   const Vec2D coords_rel)
    : motion_(motion),
      coords_(coords),
      coords_rel_(coords_rel) {
}

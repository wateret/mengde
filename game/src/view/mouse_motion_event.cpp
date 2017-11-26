#include "mouse_motion_event.h"

MouseMotionEvent::MouseMotionEvent(Motion motion,
                                   const Vec2D coords,
                                   const Vec2D coords_rel)
    : motion_(motion),
      coords_(coords),
      coords_rel_(coords_rel) {
}

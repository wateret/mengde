#include "mouse_wheel_event.h"

namespace mengde {
namespace gui {
namespace foundation {

MouseWheelEvent::MouseWheelEvent(Horizontal hor, Vertical ver, Vec2D coords) : hor_(hor), ver_(ver), coords_(coords) {
}

MouseWheelEvent::MouseWheelEvent(const MouseWheelEvent& e, Vec2D new_coords)
    : hor_(e.hor_), ver_(e.ver_), coords_(new_coords) {
}

} // namespace foundation
} // namespace gui
} // namespace mengde

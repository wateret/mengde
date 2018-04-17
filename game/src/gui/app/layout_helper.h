#ifndef MENGDE_GUI_APP_LAYOUT_HELPER_H_
#define MENGDE_GUI_APP_LAYOUT_HELPER_H_

#include "util/vec2d.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace app {

namespace layout {

Vec2D CalcPositionNearUnit(Vec2D element_size, Vec2D frame_size, Vec2D camera_coords, Vec2D unit_cell);

}  // namespace layout

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_LAYOUT_HELPER_H_

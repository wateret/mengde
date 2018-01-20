#ifndef VIEW_APP_LAYOUT_HELPER_H_
#define VIEW_APP_LAYOUT_HELPER_H_

#include "util/vec2d.h"

namespace layout {

Vec2D CalcPositionNearUnit(Vec2D element_size, Vec2D frame_size, Vec2D camera_coords, Vec2D unit_cell);

} // namespace layout

#endif // VIEW_APP_LAYOUT_HELPER_H_

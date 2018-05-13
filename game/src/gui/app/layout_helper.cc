#include "layout_helper.h"

#include "gui/foundation/rect.h"
#include "util/common.h"

namespace mengde {
namespace gui {
namespace app {

namespace layout {

Vec2D CalcPositionNearUnit(Vec2D element_size, Vec2D frame_size, Vec2D camera_coords, Vec2D unit_cell) {
  const int kCellSize = 48;  // FIXME hardcoded cell size
  Vec2D cands[] = {(unit_cell * kCellSize - Vec2D(element_size.x, 0)),
                   (unit_cell * kCellSize - Vec2D(element_size.x - kCellSize, element_size.y)),
                   (unit_cell * kCellSize + Vec2D(kCellSize, kCellSize - element_size.y)),
                   (unit_cell * kCellSize + Vec2D(0, kCellSize))};
  Rect frame({0, 0}, frame_size);
  for (auto e : cands) {
    Vec2D calc_lt = e - camera_coords;
    Vec2D calc_rb = calc_lt + element_size;
    if (frame.Contains(calc_lt) && frame.Contains(calc_rb)) {
      return calc_lt;
    }
  }
  UNREACHABLE("Cannot arrange with given criteria");
  return {0, 0};
}

}  // namespace layout

}  // namespace app
}  // namespace gui
}  // namespace mengde

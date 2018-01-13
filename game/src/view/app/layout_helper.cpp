#include "layout_helper.h"

#include "view/foundation/rect.h"
#include "util/common.h"

namespace layout {

Vec2D CalcPositionNearUnit(Vec2D element_size, Vec2D frame_size, Vec2D camera_coords, Vec2D unit_cell) {
  const int kCellSize = 48; // FIXME hardcoded cell size
  Vec2D cands[] = {(unit_cell + Vec2D(1, 0)) * kCellSize,
                   (unit_cell + Vec2D(0, 1)) * kCellSize,
                   (unit_cell * kCellSize - Vec2D(element_size.x, element_size.y - kCellSize)),
                   (unit_cell * kCellSize - Vec2D(element_size.x - kCellSize, element_size.y))};
  Rect frame({0, 0}, frame_size);
  for (auto e : cands) {
    Vec2D calc_lt = e - camera_coords;
    Vec2D calc_rb = calc_lt + element_size;
    if (frame.Contains(calc_lt) && frame.Contains(calc_rb)) {
      return calc_lt;
    }
  }
  UNREACHABLE("Cannot arrange the  with given criteria");
  return {0, 0};
}

} // namespace layout

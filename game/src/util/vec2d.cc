#include "vec2d.h"
#include <algorithm>

Vec2D Vec2D::Bound(const Vec2D& lo, const Vec2D& up) const {
  return Vec2D(std::max(lo.x, std::min(up.x, x)), std::max(lo.y, std::min(up.y, y)));
}

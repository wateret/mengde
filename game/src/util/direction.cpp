#include "direction.h"
#include "common.h"

Direction Vec2DRelativePosition(Vec2D a, Vec2D b) {
  if (a.x - b.x < 0) return kDirRight;
  if (a.x - b.x > 0) return kDirLeft;
  if (a.y - b.y < 0) return kDirDown;
  if (a.y - b.y > 0) return kDirUp;
  return kDirNone;
}

Direction OppositeDirection(Direction d) {
  switch (d) {
    case kDirRight:
      return kDirLeft;
    case kDirLeft:
      return kDirRight;
    case kDirUp:
      return kDirDown;
    case kDirDown:
      return kDirUp;
    default:
      UNREACHABLE("Invalid value of Direction");
      return kDirNone;
  }
}

Vec2D GenerateVec2DOffset(Direction d, int o) {
  switch (d) {
    case kDirRight:
      return {o, 0};
    case kDirLeft:
      return {-o, 0};
    case kDirUp:
      return {0, -o};
    case kDirDown:
      return {0, o};
    default:
      UNREACHABLE("Invalid value of Direction");
      return {0, 0};
  }
}

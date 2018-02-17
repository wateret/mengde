#ifndef DIRECTION_H_
#define DIRECTION_H_

#include "vec2d.h"

// Direction (4 directions)
enum Direction { kDirNone, kDirLeft, kDirRight, kDirUp, kDirDown };

Direction Vec2DRelativePosition(Vec2D, Vec2D);
Direction OppositeDirection(Direction);
Vec2D     GenerateVec2DOffset(Direction, int);

#endif  // DIRECTION_H_

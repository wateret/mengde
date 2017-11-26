#include "layout_helper.h"

Rect LayoutHelper::CalcPosition(Vec2D cont_size,
                                Vec2D elem_size,
                                Align alignment,
                                int margin) {
  static const int mask = 0x03;
  static const int shift = 2;
  Vec2D pos(0, 0);

  switch (alignment & mask) {
    case kAlignNone:
    case kAlignLft:
      pos.x = margin;
      break;
    case kAlignHMid:
      pos.x = (cont_size.x - elem_size.x) / 2;
      break;
    case kAlignRgt:
      pos.x = cont_size.x - elem_size.x - margin;
      break;
    default:
      UNREACHABLE("Invalid vertical horizontal value");
      break;
  }

  switch (alignment & (mask << shift)) {
    case kAlignNone:
    case kAlignTop:
      pos.y = margin;
      break;
    case kAlignVMid:
      pos.y = (cont_size.y - elem_size.y) / 2;
      break;
    case kAlignBot:
      pos.y = cont_size.y - elem_size.y - margin;
      break;
    default:
      UNREACHABLE("Invalid vertical value");
      break;
  }

  return Rect(pos, elem_size);
}

Rect LayoutHelper::CalcPosition(const Rect* cont_rect,
                                Vec2D elem_size,
                                Align alignment,
                                int margin) {
  Rect ret = CalcPosition(cont_rect->GetSize(),
                          elem_size,
                          alignment,
                          margin);
  ret.Move(cont_rect->GetPos());
  return ret;
}


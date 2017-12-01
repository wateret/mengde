#include "rect_view.h"
#include "../drawer.h"

RectView::RectView(const Rect* frame, Color color, int border)
    : View(frame), color_(color), border_(border) {
  SetBgColor(color_);
  SetPadding(0);
}

void RectView::Render(Drawer* drawer) {
  Rect r(GetFrameCoords(), GetActualFrameSize());
  drawer->SetDrawColor({255, 255, 255, 192});
  drawer->DrawRectAbs(&r, border_);
}

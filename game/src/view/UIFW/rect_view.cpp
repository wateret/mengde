#include "rect_view.h"
#include "view/Foundation/drawer.h"

RectView::RectView(const Rect* frame, Color color)
    : View(frame), color_(color), border_(0), border_color_(COLOR("transparent")) {
  SetBgColor(color_);
  SetPadding(0);
}

void RectView::SetBorder(int width, Color color) {
  border_ = width;
  border_color_ = color;
}

void RectView::Render(Drawer* drawer) {
  drawer->SetDrawColor(border_color_);
  drawer->DrawRect(GetFrame(), border_);
}

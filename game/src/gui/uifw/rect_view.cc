#include "rect_view.h"
#include "drawer.h"

namespace mengde {
namespace gui {
namespace uifw {

RectView::RectView(const Rect* frame, Color color)
    : View(frame), color_(color), border_(0), border_color_(COLOR("transparent")) {
  bg_color(color_);
  padding(0);
}

void RectView::SetBorder(int width, Color color) {
  border_ = width;
  border_color_ = color;
}

void RectView::Render(Drawer* drawer) {
  drawer->SetDrawColor(border_color_);
  drawer->DrawRect(GetFrame(), border_);
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

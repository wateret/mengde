#include "scroll_view.h"
#include "drawer.h"

ScrollView::ScrollView(const Rect& frame, View* view) : ViewDecorator(frame, view), coords_(0, 0) {
  SetBgColor(COLOR_RED);
  if (frame.GetSize() > view->GetFrame()->GetSize()) {
    LOG_WARNING("Wrapped frame size is smaller than ScrollView frame");
  }
  view_->SetCoords({0, 0});
}

void ScrollView::Render(Drawer* drawer) {
  RENDER_BEGIN(this);
  drawer->SubOffset(coords_);
  view_->Render(drawer);
  drawer->AddOffset(coords_);
  RENDER_END(this);
}

bool ScrollView::OnMouseButtonEvent(const MouseButtonEvent e) {
  return view_->OnMouseButtonEvent(e);
}

bool ScrollView::OnMouseMotionEvent(const MouseMotionEvent e) {
  MouseMotionEvent e_mod(e, coords_);
  return view_->OnMouseMotionEvent(e_mod);
}

bool ScrollView::OnMouseWheelEvent(const MouseWheelEvent e) {
  LOG_DEBUG("Wheel Event");
  if (e.IsUp()) {
    coords_.y += kDefaultScrollAmount;
    coords_.y = std::min(coords_.y, 0);
  }
  if (e.IsDown()) {
    coords_.y -= kDefaultScrollAmount;
    int y_min = std::min(0, GetFrame()->GetH() - view_->GetFrame()->GetH());
    coords_.y = std::max(coords_.y, y_min);
  }
  return true;
}

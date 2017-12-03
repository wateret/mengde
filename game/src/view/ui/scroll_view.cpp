#include "scroll_view.h"

ScrollView::ScrollView(const Rect& frame, View* view) : ViewDecorator(frame, view), coords_(0, 0) {
  if (frame.GetSize() > view->GetFrame()->GetSize()) {
    LOG_WARNING("Wrapped frame size is smaller than ScrollView frame");
  }
}

void ScrollView::Render(Drawer*) {
}

bool ScrollView::OnMouseButtonEvent(const MouseButtonEvent e) {
  return view_->OnMouseButtonEvent(e);
}

bool ScrollView::OnMouseMotionEvent(const MouseMotionEvent e) {
  return view_->OnMouseMotionEvent(e);
}

bool ScrollView::OnMouseWheelEvent(const MouseWheelEvent e) {
  if (e.IsUp()) {
    view_->Move({0, -kDefaultScrollAmount});
  }
  if (e.IsDown()) {
    view_->Move({0, kDefaultScrollAmount});
  }
}

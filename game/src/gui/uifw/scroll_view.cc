#include "scroll_view.h"
#include "drawer.h"

namespace mengde {
namespace gui {
namespace uifw {

ScrollView::ScrollView(const Rect& frame, View* view) : ViewDecorator(frame, view), coords_(0, 0) {
  if (frame.GetSize() > view->GetFrame()->GetSize()) {
    LOG_WARNING("Wrapped frame size is smaller than ScrollView frame");
  }
  view_->SetCoords({0, 0});
}

bool ScrollView::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) { return view_->DelegateMouseButtonEvent(e); }

bool ScrollView::OnMouseMotionEvent(const foundation::MouseMotionEvent& e) { return view_->DelegateMouseMotionEvent(e); }

bool ScrollView::OnMouseWheelEvent(const foundation::MouseWheelEvent& e) {
  Vec2D coords_o = coords_;
  if (e.IsLeft()) {
    coords_.x += kDefaultScrollAmount;
    coords_.x = std::min(coords_.x, 0);
  } else if (e.IsRight()) {
    coords_.x -= kDefaultScrollAmount;
    int x_min = std::min(0, GetFrame()->GetW() - view_->GetFrame()->GetW());
    coords_.x = std::max(coords_.x, x_min);
  }
  if (e.IsUp()) {
    coords_.y += kDefaultScrollAmount;
    coords_.y = std::min(coords_.y, 0);
  } else if (e.IsDown()) {
    coords_.y -= kDefaultScrollAmount;
    int y_min = std::min(0, GetFrame()->GetH() - view_->GetFrame()->GetH());
    coords_.y = std::max(coords_.y, y_min);
  }
  view_->SetCoords(coords_);

  OnMouseMotionEvent(foundation::MouseMotionEvent(MouseMotionEvent::Type::kOver, e.GetCoords(), coords_o - coords_));
  return true;
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

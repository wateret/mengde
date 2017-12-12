#include "scroll_view.h"
#include "drawer.h"

ScrollView::ScrollView(const Rect& frame, View* view) : ViewDecorator(frame, view), coords_(0, 0) {
  if (frame.GetSize() > view->GetFrame()->GetSize()) {
    LOG_WARNING("Wrapped frame size is smaller than ScrollView frame");
  }
  view_->SetCoords({0, 0});

  // XXX Remove this workaround for padding
  SetPadding(view_->GetPadding());
  view_->SetPadding(0);
  SetBgColor(view_->GetBgColor());
  view_->SetBgColor({255, 255, 255, 0});
}

void ScrollView::Render(Drawer* drawer) {
  RENDER_BEGIN(this);
  view_->Render(drawer);
  RENDER_END(this);
}

bool ScrollView::OnMouseButtonEvent(const MouseButtonEvent e) {
  return view_->OnMouseButtonEvent(e);
}

bool ScrollView::OnMouseMotionEvent(const MouseMotionEvent e) {
  MouseMotionEvent e_mod(e, -coords_);
//  LOG_DEBUG("coords_ : %d %d / Coords Mod : %d %d", coords_.x, coords_.y, e_mod.GetCoords().x, e_mod.GetCoords().y);
  return view_->OnMouseMotionEvent(e_mod);
}

bool ScrollView::OnMouseWheelEvent(const MouseWheelEvent e) {
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
  OnMouseMotionEvent(MouseMotionEvent(MouseMotionEvent::Type::kOver, e.GetCoords(), coords_ - coords_o));
  return true;
}
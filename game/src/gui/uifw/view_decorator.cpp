#include "view_decorator.h"

namespace mengde {
namespace gui {
namespace uifw {

ViewDecorator::ViewDecorator(View* view) : View(view->GetFrame()), view_(view) {
}

ViewDecorator::ViewDecorator(const Rect& frame, View* view) : View(frame), view_(view) {
}

ViewDecorator::~ViewDecorator() {
  delete view_;
}

void ViewDecorator::Render(Drawer* drawer) {
  RENDER_BEGIN(this);
  view_->Render(drawer);
  RENDER_END(this);
}

void ViewDecorator::Update() {
  view_->Update();
}

bool ViewDecorator::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  return view_->OnMouseButtonEvent(e);
}

bool ViewDecorator::OnMouseMotionEvent(const foundation::MouseMotionEvent e) {
  return view_->OnMouseMotionEvent(e);
}

bool ViewDecorator::OnMouseWheelEvent(const foundation::MouseWheelEvent e) {
  return view_->OnMouseWheelEvent(e);
}

} // namespace uifw
} // namespace gui
} // namespace mengde

#include "view_decorator.h"

ViewDecorator::ViewDecorator(View* view) : View(view->GetFrame()), view_(view) {
}

ViewDecorator::ViewDecorator(const Rect& frame, View* view) : View(frame), view_(view) {
}

ViewDecorator::~ViewDecorator() {
  delete view_;
}

void ViewDecorator::Render(Drawer* drawer) {
  view_->Render(drawer);
}

void ViewDecorator::Update() {
  view_->Update();
}

bool ViewDecorator::OnMouseButtonEvent(const MouseButtonEvent e) {
  return view_->OnMouseButtonEvent(e);
}

bool ViewDecorator::OnMouseMotionEvent(const MouseMotionEvent e) {
  return view_->OnMouseMotionEvent(e);
}

bool ViewDecorator::OnMouseWheelEvent(const MouseWheelEvent e) {
  return view_->OnMouseWheelEvent(e);
}

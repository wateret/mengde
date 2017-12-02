#include "view_decorator.h"

ViewDecorator::ViewDecorator(View* view) : view_(view) {
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

#include "i_view_decorator.h"

IViewDecorator::IViewDecorator(IView* iview) : iview_(iview) {
}

void IViewDecorator::Render(Drawer* drawer) {
  iview_->Render(drawer);
}

void IViewDecorator::Update() {
  iview_->Update();
}

bool IViewDecorator::OnMouseButtonEvent(const MouseButtonEvent e) {
  return iview_->OnMouseButtonEvent(e);
}

bool IViewDecorator::OnMouseMotionEvent(const MouseMotionEvent e) {
  return iview_->OnMouseMotionEvent(e);
}

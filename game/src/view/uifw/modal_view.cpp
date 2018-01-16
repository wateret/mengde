#include "modal_view.h"

ModalView::ModalView(const Rect& frame, View* view, const MouseButtonHandler& handler)
    : ViewDecorator(frame, view), handler_(handler) {
}

bool ModalView::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (view_->DelegateMouseButtonEvent(e)) return true;
  return handler_(e);
}

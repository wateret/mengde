#include "modal_view.h"

namespace mengde {
namespace gui {
namespace uifw {

ModalView::ModalView(const Rect& frame, View* view, const MouseButtonHandler& handler)
    : ViewDecorator(frame, view), handler_(handler) {
}

bool ModalView::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (view_->DelegateMouseButtonEvent(e)) return true;
  return handler_(e);
}

} // namespace uifw
} // namespace gui
} // namespace mengde

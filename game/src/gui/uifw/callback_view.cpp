#include "callback_view.h"

namespace mengde {
namespace gui {
namespace uifw {

CallbackView::CallbackView() : CompositeView() {
  InitHandlers();
}

CallbackView::CallbackView(const Rect* rect) : CallbackView(*rect) {
}

CallbackView::CallbackView(const Rect& rect) : CompositeView(rect) {
  InitHandlers();
}

void CallbackView::InitHandlers() {
  mouse_button_handler_ = [] (const foundation::MouseButtonEvent) { return false; };
  mouse_motion_handler_ = [] (const foundation::MouseMotionEvent) { return false; };
}

bool CallbackView::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  ASSERT(mouse_button_handler_);
  return mouse_button_handler_(e);
}

bool CallbackView::OnMouseMotionEvent(const foundation::MouseMotionEvent e) {
  ASSERT(mouse_motion_handler_);
  return mouse_motion_handler_(e);
}

void CallbackView::SetMouseButtonHandler(MouseButtonHandler handler) {
  mouse_button_handler_ = handler;
}

void CallbackView::SetMouseMotionHandler(MouseMotionHandler handler) {
  mouse_motion_handler_ = handler;
}

} // namespace uifw
} // namespace gui
} // namespace mengde


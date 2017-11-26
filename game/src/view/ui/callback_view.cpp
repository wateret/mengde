#include "callback_view.h"

CallbackView::CallbackView() : View() {
  InitHandlers();
}

CallbackView::CallbackView(const Rect* rect) : View(rect) {
  InitHandlers();
}

void CallbackView::InitHandlers() {
  mouse_button_handler_ = [] (const MouseButtonEvent) { return false; };
  mouse_motion_handler_ = [] (const MouseMotionEvent) { return false; };
}

bool CallbackView::OnMouseButtonEvent(const MouseButtonEvent e) {
  ASSERT(mouse_button_handler_);
  return mouse_button_handler_(e);
}

bool CallbackView::OnMouseMotionEvent(const MouseMotionEvent e) {
  ASSERT(mouse_motion_handler_);
  return mouse_motion_handler_(e);
}

void CallbackView::SetMouseButtonHandler(MouseButtonHandler handler) {
  mouse_button_handler_ = handler;
}

void CallbackView::SetMouseMotionHandler(MouseMotionHandler handler) {
  mouse_motion_handler_ = handler;
}


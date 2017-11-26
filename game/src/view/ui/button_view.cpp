#include "button_view.h"

ButtonView::ButtonView(const Rect* frame, const std::string& text)
    : CallbackView(frame), text_(text), hover_color_(0,0,0,255) {
  SetBgColor(COLOR_GRAY);
  SetMouseMotionHandler([this] (const MouseMotionEvent e) -> bool {
    if (e.IsMotionOver()) {
      SetBgColor(COLOR_BLACK);
    } else {
      SetBgColor(COLOR_GRAY);
    }
    return true;
  });
}

void ButtonView::RenderView(Drawer* drawer) {
  Rect frame = GetActualFrame();
  drawer->DrawTextAbs(text_, 14, COLOR_WHITE, &frame, LayoutHelper::kAlignCenter);
}


#include "button_view.h"
#include "text_view.h"
#include "../drawer.h"

ButtonView::ButtonView(const Rect* frame, const std::string& text)
    : CallbackView(frame), tv_label_(nullptr), hover_color_(0,0,0,255) {
  SetBgColor(COLOR_GRAY);

  Rect rect_label = GetActualFrame();
  tv_label_= new TextView(&rect_label, text, COLOR_WHITE, 14, LayoutHelper::kAlignCenter);
  SetMouseMotionHandler([this] (const MouseMotionEvent e) -> bool {
    if (e.IsMotionOver()) {
      SetBgColor(COLOR_BLACK);
    } else {
      SetBgColor(COLOR_GRAY);
    }
    return true;
  });
}

void ButtonView::Render(Drawer* drawer) {
  // FIXME This class must inherit CompositeView and not override Render function
  RENDER_BEGIN(this);
  tv_label_->Render(drawer);
  RENDER_END(this);
}


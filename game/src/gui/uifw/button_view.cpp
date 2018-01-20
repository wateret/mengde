#include "button_view.h"
#include "text_view.h"
#include "drawer.h"

namespace mengde {
namespace gui {
namespace uifw {

ButtonView::ButtonView(const Rect* frame, const std::string& text)
    : CallbackView(frame), tv_label_(nullptr), hover_color_(0,0,0,255) {
  bg_color(COLOR("gray"));

  Rect rect_label = GetActualFrame();
  tv_label_= new TextView(&rect_label, text, COLOR("white"), 14, LayoutHelper::kAlignCenter);
  AddChild(tv_label_);
  SetMouseMotionHandler([this] (const MouseMotionEvent e) -> bool {
    if (e.IsMotionOver()) {
      bg_color(COLOR(32, 32, 32));
    } else {
      bg_color(COLOR("gray"));
    }
    return true;
  });
}

} // namespace uifw
} // namespace gui
} // namespace mengde


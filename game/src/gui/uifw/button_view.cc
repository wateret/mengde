#include "button_view.h"
#include "drawer.h"
#include "text_view.h"

namespace mengde {
namespace gui {
namespace uifw {

ButtonView::ButtonView(const Rect* frame, const std::string& text) : ButtonView(*frame, text) {}

ButtonView::ButtonView(const Rect& frame, const std::string& text)
    : CallbackView(frame),
      checked_(false),
      tv_label_(nullptr),
      base_color_(COLOR("gray")),
      hover_color_(96, 96, 96),
      checked_color_(COLOR(32, 32, 32)) {
  bg_color(GetNormalColor());

  Rect rect_label = GetActualFrame();
  tv_label_ = new TextView(rect_label, text, COLOR("white"), 14, LayoutHelper::kAlignCenter);
  AddChild(tv_label_);
  SetMouseMotionHandler([=](const foundation::MouseMotionEvent& e) -> bool {
    if (e.IsMotionOver()) {
      this->bg_color(checked() ? GetNormalColor() : hover_color_);
    } else {
      this->bg_color(GetNormalColor());
    }
    return true;
  });
}

void ButtonView::check(bool b) {
  checked_ = b;
  bg_color(GetNormalColor());
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

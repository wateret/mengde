#ifndef BUTTON_VIEW_H_
#define BUTTON_VIEW_H_

#include "callback_view.h"
#include "util/common.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class TextView;

class ButtonView : public CallbackView {
 public:
  ButtonView(const Rect*, const std::string& = "");
  bool checked() const { return checked_; }
  void check(bool);

 private:
  const Color& GetNormalColor() { return checked_ ? checked_color_ : base_color_; }

 private:
  bool      checked_;
  TextView* tv_label_;
  Color     base_color_;
  Color     hover_color_;
  Color     checked_color_;
};

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // BUTTON_VIEW_H_

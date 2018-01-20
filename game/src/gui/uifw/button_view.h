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

 private:
  TextView* tv_label_;
  Color     hover_color_;
};

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // BUTTON_VIEW_H_

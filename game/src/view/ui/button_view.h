#ifndef BUTTON_VIEW_H_
#define BUTTON_VIEW_H_

#include "callback_view.h"
#include "common.h"

class TextView;

class ButtonView : public CallbackView {
 public:
  ButtonView(const Rect*, const std::string& = "");
  virtual void Render(Drawer*) override;

 private:
  TextView* tv_label_;
  Color     hover_color_;
};

#endif // BUTTON_VIEW_H_

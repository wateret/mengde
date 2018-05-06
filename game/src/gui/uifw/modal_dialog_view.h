#ifndef MENGDE_GUI_UIFW_MODAL_DIALOG_VIEW_H_
#define MENGDE_GUI_UIFW_MODAL_DIALOG_VIEW_H_

#include "composite_view.h"
#include "util/common.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class TextView;

// ModalDialogView

class ModalDialogView : public CompositeView {
 public:
  // Usually the value of frame that passed as first argument of the constructor is ({0, 0}, WindowSize)
  // so we can handle mouse click events anywhere in the screen.
  ModalDialogView(const Rect*, const string&);
  void SetText(const string&);

 private:
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;

 private:
  string         message_;
  CompositeView* frame_view_;
  TextView*      message_view_;
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_UIFW_MODAL_DIALOG_VIEW_H_

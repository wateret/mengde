#ifndef MODAL_DIALOG_VIEW_H_
#define MODAL_DIALOG_VIEW_H_

#include "common.h"
#include "composite_view.h"

class TextView;

// ModalDialogView

class ModalDialogView : public CompositeView {
 public:
  // Usually the value of frame that passed as first argument of the constructor is ({0, 0}, WindowSize) 
  // so we can handle mouse click events anywhere in the screen.
  ModalDialogView(const Rect*, const string&);
  void SetText(const string&);

 private:
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  string message_;
  CompositeView* frame_view_;
  TextView* message_view_;
};

#endif // MODAL_DIALOG_VIEW_H_

#ifndef MODAL_VIEW_H_
#define MODAL_VIEW_H_

#include "view_decorator.h"

class ModalView : public ViewDecorator {
 public:
  typedef std::function<bool(const MouseButtonEvent)> MouseButtonHandler;

 public:
  ModalView(const Rect& frame, View* view, const MouseButtonHandler&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;

 private:
  MouseButtonHandler handler_;
};

#endif // MODAL_VIEW_H_

#ifndef CALLBACK_VIEW_H_
#define CALLBACK_VIEW_H_

#include "composite_view.h"
#include <functional>

class CallbackView : public CompositeView {
 public:
  typedef std::function<bool(const MouseButtonEvent)> MouseButtonHandler;
  typedef std::function<bool(const MouseMotionEvent)> MouseMotionHandler;

 public:
  CallbackView();
  CallbackView(const Rect*);
  CallbackView(const Rect&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override final;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override final;
  void SetMouseButtonHandler(MouseButtonHandler);
  void SetMouseMotionHandler(MouseMotionHandler);

 private:
  void InitHandlers();

 private:
  MouseButtonHandler mouse_button_handler_;
  MouseMotionHandler mouse_motion_handler_;
};

#endif // CALLBACK_VIEW_H_

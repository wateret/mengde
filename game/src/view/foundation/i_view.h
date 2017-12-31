#ifndef I_VIEW_H_
#define I_VIEW_H_

#include "mouse_button_event.h"
#include "mouse_motion_event.h"
#include "mouse_wheel_event.h"

class Drawer;

class IView {
 public:
  virtual ~IView() {}
  virtual void Render(Drawer*) = 0;
  virtual void Update() = 0;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) = 0;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) = 0;
  virtual bool OnMouseWheelEvent(const MouseWheelEvent) = 0;

 private:
};

#endif // I_VIEW_H_

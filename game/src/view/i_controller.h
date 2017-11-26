#ifndef I_CONTROLLER_H_
#define I_CONTROLLER_H_

#include "mouse_button_event.h"
#include "mouse_motion_event.h"

class IController {
 public:
  virtual ~IController() {}
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) = 0;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) = 0;
};

#endif // I_CONTROLLER_H_

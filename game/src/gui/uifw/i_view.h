#ifndef MENGDE_GUI_UIFW_I_VIEW_H_
#define MENGDE_GUI_UIFW_I_VIEW_H_

#include "gui/foundation/key_event.h"
#include "gui/foundation/mouse_button_event.h"
#include "gui/foundation/mouse_motion_event.h"
#include "gui/foundation/mouse_wheel_event.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class Drawer;

class IView {
 public:
  virtual ~IView() {}
  virtual void Render(Drawer*) = 0;
  virtual void Update() = 0;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) = 0;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) = 0;
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent&) = 0;
  virtual bool OnKeyEvent(const foundation::KeyEvent&) = 0;

 private:
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_UIFW_I_VIEW_H_

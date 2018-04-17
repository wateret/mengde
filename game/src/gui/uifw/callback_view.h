#ifndef MENGDE_GUI_UIFW_CALLBACK_VIEW_H_
#define MENGDE_GUI_UIFW_CALLBACK_VIEW_H_

#include <functional>
#include "composite_view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class CallbackView : public CompositeView {
 public:
  typedef std::function<bool(const foundation::MouseButtonEvent)> MouseButtonHandler;
  typedef std::function<bool(const foundation::MouseMotionEvent)> MouseMotionHandler;

 public:
  CallbackView();
  CallbackView(const Rect*);
  CallbackView(const Rect&);
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override final;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent) override final;
  void         SetMouseButtonHandler(MouseButtonHandler);
  void         SetMouseMotionHandler(MouseMotionHandler);

 private:
  void InitHandlers();

 private:
  MouseButtonHandler mouse_button_handler_;
  MouseMotionHandler mouse_motion_handler_;
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_UIFW_CALLBACK_VIEW_H_

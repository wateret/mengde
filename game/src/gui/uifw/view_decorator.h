#ifndef MENGDE_GUI_UIFW_VIEW_DECORATOR_H_
#define MENGDE_GUI_UIFW_VIEW_DECORATOR_H_

#include "view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class ViewDecorator : public View {
 public:
  ViewDecorator(View*);
  ViewDecorator(const Rect&, View*);
  virtual ~ViewDecorator();
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent&) override;

 protected:
  View* view_;
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_UIFW_VIEW_DECORATOR_H_

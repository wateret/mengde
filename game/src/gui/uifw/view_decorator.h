#ifndef VIEW_DECORATOR_H_
#define VIEW_DECORATOR_H_

#include "view.h"

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
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const MouseWheelEvent) override;

 protected:
  View* view_;
};

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // VIEW_DECORATOR_H_

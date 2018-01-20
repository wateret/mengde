#ifndef SCROLL_VIEW_H_
#define SCROLL_VIEW_H_

#include "view_decorator.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class Drawer;

class ScrollView : public ViewDecorator {
 public:
  const int kDefaultScrollAmount = 6;

 public:
  ScrollView(const Rect&, View*);
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent) override;

 private:
  Vec2D coords_;
};

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // SCROLL_VIEW_H_

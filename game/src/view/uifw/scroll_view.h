#ifndef SCROLL_VIEW_H_
#define SCROLL_VIEW_H_

#include "view/foundation/view_decorator.h"

class Drawer;

class ScrollView : public ViewDecorator {
 public:
  const int kDefaultScrollAmount = 6;

 public:
  ScrollView(const Rect&, View*);
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const MouseWheelEvent) override;

 private:
  Vec2D coords_;
};

#endif // SCROLL_VIEW_H_

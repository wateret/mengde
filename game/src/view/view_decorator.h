#ifndef VIEW_DECORATOR_H_
#define VIEW_DECORATOR_H_

#include "view.h"

class ViewDecorator : public View {
 public:
  ViewDecorator(View*);
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const MouseWheelEvent) override;

 private:
  View* view_;
};

#endif // VIEW_DECORATOR_H_

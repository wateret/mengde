#ifndef I_VIEW_DECORATOR_H_
#define I_VIEW_DECORATOR_H_

#include "i_view.h"

class IViewDecorator : public IView {
 public:
  IViewDecorator(IView*);
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  IView* iview_;
};

#endif // I_VIEW_DECORATOR_H_

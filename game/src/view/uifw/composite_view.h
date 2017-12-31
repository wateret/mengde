#ifndef COMPOSITE_VIEW_H_
#define COMPOSITE_VIEW_H_

#include "view/foundation/view.h"
#include <vector>

class CompositeView : public View {
 public:
  CompositeView();
  CompositeView(const Rect*);
  CompositeView(const Rect&);
  virtual ~CompositeView();
  int   AddChild(View*);
  void  RemoveChild(int);
  void  RemoveChild(View*);
  void  RemoveAllChildren();
  void  SetTransparent() { transparent_ = true; }
  void  SetOpaque() { transparent_ = false; }

 public:
  void Render(Drawer*) override;
  void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const MouseWheelEvent) override;

 private:
  std::vector<View*> children_;
  bool transparent_;
};

#endif // COMPOSITE_VIEW_H_

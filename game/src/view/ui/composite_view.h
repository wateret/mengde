#ifndef COMPOSITE_VIEW_H_
#define COMPOSITE_VIEW_H_

#include "view.h"
#include <vector>

class CompositeView : public View {
 public:
  CompositeView();
  CompositeView(const Rect*);
  virtual ~CompositeView();
  int   AddChild(View*);
  void  RemoveChild(int);

 public:
  void RenderView(Drawer*) override;
  void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  std::vector<View*> children_;
};

#endif // COMPOSITE_VIEW_H_

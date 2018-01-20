#ifndef COMPOSITE_VIEW_H_
#define COMPOSITE_VIEW_H_

#include "view.h"
#include <vector>

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

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
  void  ReplaceChild(View*, View*);
  void  SetTransparent() { transparent_ = true; }
  void  SetOpaque() { transparent_ = false; }

 public:
  void Render(Drawer*) override;
  void Update() override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent) override;

 private:
  std::vector<View*> children_;
  bool transparent_;
};

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // COMPOSITE_VIEW_H_

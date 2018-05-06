#ifndef MENGDE_GUI_UIFW_COMPOSITE_VIEW_H_
#define MENGDE_GUI_UIFW_COMPOSITE_VIEW_H_

#include <vector>
#include "view.h"

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
  int  AddChild(View*);
  void RemoveChild(int);
  void RemoveChild(View*);
  void RemoveAllChildren();
  void ReplaceChild(View*, View*);
  void SetTransparent() { transparent_ = true; }
  void SetOpaque() { transparent_ = false; }

 public:
  void         Render(Drawer*) override;
  void         Update() override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent&) override;

 private:
  std::vector<View*> children_;
  bool               transparent_;  // TODO Add comment what is this for. I don't remember why ':)
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_UIFW_COMPOSITE_VIEW_H_

#ifndef MENGDE_GUI_UIFW_RECT_VIEW_H_
#define MENGDE_GUI_UIFW_RECT_VIEW_H_

#include "util/common.h"
#include "view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class RectView : public View {
 public:
  RectView(const Rect&, Color);
  void SetBorder(int, Color);

 private:
  virtual void Render(Drawer*) override;

 private:
  Color color_;
  int border_;
  Color border_color_;
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_UIFW_RECT_VIEW_H_

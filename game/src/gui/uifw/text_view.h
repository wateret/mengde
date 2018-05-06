#ifndef MENGDE_GUI_UIFW_TEXT_VIEW_H_
#define MENGDE_GUI_UIFW_TEXT_VIEW_H_

#include "layout_helper.h"
#include "util/common.h"
#include "view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class TextView : public View {
 public:
  TextView(const Rect*, const std::string& = "");
  TextView(const Rect*, const std::string&, Color, int, LayoutHelper::Align);
  void SetText(const std::string& text);
  void SetTextSize(int size) { size_ = size; }
  void SetAlign(LayoutHelper::Align align);
  void SetColor(Color);

  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;

 private:
  void UpdateCoords();

 private:
  std::string         text_;
  Color               color_;
  int                 size_;
  LayoutHelper::Align align_;
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_UIFW_TEXT_VIEW_H_

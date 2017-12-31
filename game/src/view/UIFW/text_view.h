#ifndef TEXT_VIEW_H_
#define TEXT_VIEW_H_

#include "util/common.h"
#include "view/Foundation/view.h"
#include "view/Foundation/layout_helper.h"

class TextView : public View {
 public:
  TextView(const Rect*, const std::string& = "");
  TextView(const Rect*, const std::string&, Color, int, LayoutHelper::Align);
  void SetText(const std::string& text);
  void SetTextSize(int size) { size_ = size; }
  void SetAlign(LayoutHelper::Align align);
  void SetColor(Color);

  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  void UpdateCoords();

 private:
  std::string text_;
  Color       color_;
  int         size_;
  LayoutHelper::Align align_;
};

#endif // TEXT_VIEW_H_

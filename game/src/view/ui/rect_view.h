#ifndef RECT_VIEW_H_
#define RECT_VIEW_H_

#include "view.h"
#include "common.h"

class RectView : public View {
 public:
  RectView(const Rect*, Color);
  void SetBorder(int, Color);

 private:
  virtual void Render(Drawer*) override;

 private:
  Color color_;
  int   border_;
  Color border_color_;
};

#endif // RECT_VIEW_H_

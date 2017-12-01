#ifndef RECT_VIEW_H_
#define RECT_VIEW_H_

#include "view.h"
#include "common.h"

class RectView : public View {
 public:
  RectView(const Rect*, Color, int = 0);

 private:
  virtual void Render(Drawer*) override;

 private:
  Color color_;
  int   border_;
};

#endif // RECT_VIEW_H_

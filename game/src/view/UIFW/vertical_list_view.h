#ifndef VERTICAL_LIST_VIEW_H_
#define VERTICAL_LIST_VIEW_H_

#include "composite_view.h"

class VerticalListView : public CompositeView {
 public:
  VerticalListView(const Rect&);
  void AddElement(View* view);
  void SetMargin(int margin) { margin_ = margin; }

 private:
  int margin_;
};

#endif // VERTICAL_LIST_VIEW_H_

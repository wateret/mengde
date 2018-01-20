#ifndef ROW_MAJOR_LIST_VIEW_H_
#define ROW_MAJOR_LIST_VIEW_H_

#include "composite_view.h"

class RowMajorListView : public CompositeView {
 public:
  RowMajorListView(const Rect&, int);
  void AddElement(View* view);

 private:
  int row_height_;
  int last_x_;
};

#endif // ROW_MAJOR_LIST_VIEW_H_

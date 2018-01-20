#ifndef ROW_MAJOR_LIST_VIEW_H_
#define ROW_MAJOR_LIST_VIEW_H_

#include "composite_view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class RowMajorListView : public CompositeView {
 public:
  RowMajorListView(const Rect&, int);
  void AddElement(View* view);

 private:
  int row_height_;
  int last_x_;
};

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // ROW_MAJOR_LIST_VIEW_H_

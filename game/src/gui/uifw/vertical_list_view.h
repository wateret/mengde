#ifndef VERTICAL_LIST_VIEW_H_
#define VERTICAL_LIST_VIEW_H_

#include "composite_view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class VerticalListView : public CompositeView {
 public:
  VerticalListView(const Rect&);
  void AddElement(View* view);
  void SetMargin(int margin) { margin_ = margin; }

 private:
  int margin_;
  int total_element_height_;
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // VERTICAL_LIST_VIEW_H_

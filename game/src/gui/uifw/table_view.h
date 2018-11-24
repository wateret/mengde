#ifndef MENGDE_GUI_UIFW_TABLE_VIEW_H_
#define MENGDE_GUI_UIFW_TABLE_VIEW_H_

#include "composite_view.h"

#include "common.h"
#include "util/common.h"

namespace mengde {
namespace gui {
namespace uifw {

class TableView : public CompositeView {
 public:
  static const int HEADER_HEIGHT;

 public:
  TableView(const Rect &);

 public:
  int AddCol(const string &title);
  int AddRow(std::initializer_list<View *> views);
  int GetColSize();
  int GetRowSize();
  void Clear();

 private:
  vector<View *> headers_;
  vector<vector<View *>> rows_;
  int total_element_height_;
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_UIFW_TABLE_VIEW_H_

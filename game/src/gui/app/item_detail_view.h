#ifndef MENGDE_GUI_APP_ITEM_DETAIL_VIEW_
#define MENGDE_GUI_APP_ITEM_DETAIL_VIEW_

#include "gui/uifw/composite_view.h"
#include "gui/uifw/layout_helper.h"

#include "common.h"

namespace mengde {
namespace core {
class Equipment;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class ItemDetailView : public CompositeView {
 public:
  ItemDetailView(const Rect&, const core::Equipment*);

 public:
  void SetItem(const core::Equipment*);

 private:
  ImageView* iv_image_;
  TextView* tv_name_;
  TextView* tv_desc_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_ITEM_DETAIL_VIEW_

#ifndef MENGDE_GUI_APP_UNIT_STATUS_VIEW_H_
#define MENGDE_GUI_APP_UNIT_STATUS_VIEW_H_

#include "common.h"
#include "gui/uifw/vertical_list_view.h"

namespace mengde {
namespace core {
class Unit;
}
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class UnitStatusView : public uifw::VerticalListView {
 public:
  UnitStatusView(const Rect& frame);

 public:
  void SetUnit(const core::Unit* unit);

 private:
  void OnUnitUpdate();

 private:
  const core::Unit* unit_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_UNIT_STATUS_VIEW_H_

#ifndef MENGDE_GUI_APP_UNIT_VIEW_H_
#define MENGDE_GUI_APP_UNIT_VIEW_H_

#include "gui/uifw/tab_view.h"

#include "common.h"

namespace mengde {
namespace core {

class IUnitBase;
}
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class UnitOverView;
class EquipmentSetView;
class UnitStatusView;

class UnitView : public TabView {
 public:
  UnitView(const Rect&);
  void SetUnit(const core::IUnitBase* unit);
  UnitOverView* unit_over_view() { return unit_over_view_; }
  EquipmentSetView* equipment_set_view() { return equipment_set_view_; }

 private:
  const core::IUnitBase* unit_;
  UnitOverView* unit_over_view_;          // tab 0
  EquipmentSetView* equipment_set_view_;  // tab 1
  UnitStatusView* unit_status_view_;      // tab 2
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_UNIT_VIEW_H_

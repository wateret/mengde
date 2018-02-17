#ifndef UNIT_VIEW_H_
#define UNIT_VIEW_H_

#include "gui/uifw/tab_view.h"

#include "common.h"

namespace mengde {
namespace core {
class Unit;
}
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class UnitOverView;
class EquipmentSetView;

class UnitView : public TabView {
 public:
  UnitView(const Rect*);
  void SetUnit(core::Unit* unit);

 private:
  core::Unit*       unit_;
  UnitOverView*     unit_over_view_;      // tab 0
  EquipmentSetView* equipment_set_view_;  // tab 1
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // UNIT_VIEW_H_

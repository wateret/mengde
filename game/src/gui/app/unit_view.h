#ifndef UNIT_VIEW_H_
#define UNIT_VIEW_H_

#include "gui/uifw/tab_view.h"

namespace mengde {
namespace core {
  class Unit;
}
}

class UnitOverView;
class EquipmentSetView;

class UnitView : public TabView {
 public:
  UnitView(const Rect*);
  void SetUnit(mengde::core::Unit* unit);

 private:
  mengde::core::Unit* unit_;
  UnitOverView* unit_over_view_; // tab 0
  EquipmentSetView* equipment_set_view_; // tab 1
};

#endif // UNIT_VIEW_H_

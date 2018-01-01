#ifndef UNIT_VIEW_H_
#define UNIT_VIEW_H_

#include "view/uifw/tab_view.h"

class Unit;
class UnitOverView;
class EquipmentSetView;

class UnitView : public TabView {
 public:
  UnitView(const Rect*);
  void SetUnit(Unit* unit);

 private:
  Unit* unit_;
  UnitOverView* unit_over_view_; // tab 0
  EquipmentSetView* equipment_set_view_; // tab 1
};

#endif // UNIT_VIEW_H_

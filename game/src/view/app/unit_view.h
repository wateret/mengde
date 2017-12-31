#ifndef UNIT_VIEW_H_
#define UNIT_VIEW_H_

#include "view/uifw/composite_view.h"
#include "view/uifw/tab_view.h"

class Unit;
class GaugeView;
class ImageView;
class ButtonView;
class TextView;
class UnitOverView;
class UnitEquipmentView;
class EquipmentView;
class Equipment;

class UnitView : public TabView {
 public:
  UnitView(const Rect*);
  void SetUnit(Unit* unit);

 private:
  Unit* unit_;
  UnitOverView* unit_over_view_; // tab 0
  UnitEquipmentView* unit_equipment_view_; // tab 1
};

class UnitOverView : public CompositeView {
 public:
  UnitOverView(const Rect*);
  void SetUnit(Unit* unit);

 private:
  void OnUnitUpdate();

 private:
  static const int kNumXtats = 3;
  static const int kNumStats = 5;
  static const int kNumGVs   = kNumXtats + kNumStats;
  static const int kNumTabs  = 3;

 private:
  Unit*       unit_;
  ImageView*  iv_portrait_;
  TextView*   tv_name_;
  TextView*   tv_lv_;
  TextView*   tv_stats_[kNumGVs];
  GaugeView*  gv_stats_[kNumGVs];
};

#endif // UNIT_VIEW_H_

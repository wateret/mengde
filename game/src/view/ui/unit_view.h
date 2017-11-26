#ifndef UNIT_VIEW_H_
#define UNIT_VIEW_H_

#include "composite_view.h"
#include "tab_view.h"

class Unit;
class GaugeView;
class ImageView;
class ButtonView;
class TextView;
class UnitOverView;
class UnitItemView;
class ItemView;
class Item;

class UnitView : public TabView {
 public:
  UnitView(const Rect*);
  void SetUnit(Unit* unit);

 private:
  Unit* unit_;
  UnitOverView* unit_over_view_; // tab 0
  UnitItemView* unit_item_view_; // tab 1
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

class UnitItemView : public CompositeView {
 public:
  UnitItemView(const Rect*);
  void SetUnit(Unit* unit);

 private:
  void OnUnitUpdate();

 private:
  Unit*     unit_;
  TextView* tv_item_weapon_label_;
  TextView* tv_item_armor_label_;
  TextView* tv_item_aid_label_;
  ItemView* itv_weapon_;
  ItemView* itv_armor_;
  ItemView* itv_aid_;
};

class ItemView : public CompositeView {
 public:
  ItemView(const Rect*, Item*);
  void SetItem(Item*);

 private:
  ImageView* iv_image_;
  TextView*  tv_name_;
  TextView*  tv_desc_;
};

#endif // UNIT_VIEW_H_

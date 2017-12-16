#ifndef UNIT_INFO_VIEW_H_
#define UNIT_INFO_VIEW_H_

#include "composite_view.h"
#include <string>

class Unit;
class Cell;
class GaugeView;
class TextView;

class UnitInfoView : public CompositeView {
 public:
  UnitInfoView(const Rect*, Unit* = NULL);
  virtual ~UnitInfoView();
  void SetUnitTerrainInfo(Cell* cell);
  void SetUnitAttackInfo(Unit*, int, int);
  void SetUnit(Unit*);
  void SetCoordsByUnitCoords(Vec2D, Vec2D);

  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  Unit*      unit_;
  GaugeView* gv_hp_;
  GaugeView* gv_mp_;
  TextView*  tv_name_;
  TextView*  tv_lv_;
  TextView*  tv_lftbot_;
  TextView*  tv_rgtbot_;
};

#endif

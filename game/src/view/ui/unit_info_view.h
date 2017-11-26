#ifndef UNIT_INFO_VIEW_H_
#define UNIT_INFO_VIEW_H_

#include "view.h"
#include <string>

class Unit;
class Cell;
class GaugeView;

class UnitInfoView : public View {
 public:
  UnitInfoView(const Rect*, Unit* = NULL);
  virtual ~UnitInfoView();
  void SetUnitTerrainInfo(Cell* cell);
  void SetUnitAttackInfo(Unit*, int, int);
  void SetUnit(Unit*);

 private:
  virtual void RenderView(Drawer*) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  Unit*      unit_;
  GaugeView* gv_hp_;
  GaugeView* gv_mp_;
  std::string str_lftbot_;
  std::string str_rgtbot_;
};

#endif

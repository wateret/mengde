#ifndef UNIT_INFO_VIEW_H_
#define UNIT_INFO_VIEW_H_

#include "view/uifw/composite_view.h"
#include <string>

namespace mengde {
namespace core {
  class Unit;
  class Cell;
  struct HpMp;
}
}

class GaugeView;
class TextView;

class UnitInfoView : public CompositeView {
 public:
  UnitInfoView(const Rect*, mengde::core::Unit* = NULL);
  virtual ~UnitInfoView();
  void SetUnitTerrainInfo(mengde::core::Cell* cell);
  void SetUnitAttackInfo(mengde::core::Unit*, int, int);
  void SetUnit(mengde::core::Unit*);
  void SetContents(const std::string&, int, const mengde::core::HpMp&, const mengde::core::HpMp&, int = 0, int = 0);
  void SetCoordsByUnitCoords(Vec2D, Vec2D, Vec2D);

  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  mengde::core::Unit*      unit_;
  GaugeView* gv_hp_;
  GaugeView* gv_mp_;
  TextView*  tv_name_;
  TextView*  tv_lv_;
  TextView*  tv_lftbot_;
  TextView*  tv_rgtbot_;
};

#endif

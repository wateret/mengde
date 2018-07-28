#ifndef MENGDE_GUI_APP_UNIT_TOOLTIP_VIEW_H_
#define MENGDE_GUI_APP_UNIT_TOOLTIP_VIEW_H_

#include <string>
#include "gui/uifw/composite_view.h"

#include "common.h"

namespace mengde {
namespace core {
class Unit;
class Cell;
struct HpMp;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class UnitTooltipView : public CompositeView {
 public:
  UnitTooltipView(const Rect&, const core::Unit* = NULL);
  virtual ~UnitTooltipView();
  void SetUnitTerrainInfo(const core::Cell* cell, const core::Unit* unit);
  void SetUnitAttackInfo(const core::Unit*, int, int);
  void SetUnit(const core::Unit*);
  void SetContents(const std::string& name, int lv, const core::HpMp& hpmp_cur, const core::HpMp& hpmp_max,
                   const core::HpMp& hpmp_ext);
  void SetCoordsByUnitCoords(Vec2D unit, Vec2D camera, Vec2D game_frame);

  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;

 private:
  const core::Unit* unit_;

  GaugeView* gv_hp_;
  GaugeView* gv_mp_;
  TextView* tv_name_;
  TextView* tv_lv_;
  TextView* tv_lftbot_;
  TextView* tv_rgtbot_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_UNIT_TOOLTIP_VIEW_H_

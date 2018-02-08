#ifndef UNIT_INFO_VIEW_H_
#define UNIT_INFO_VIEW_H_

#include "gui/uifw/composite_view.h"
#include <string>

#include "common.h"

namespace mengde {
namespace core {
  class Unit;
  class Cell;
  struct HpMp;
}
}

namespace mengde {
namespace gui {
namespace app {

class UnitInfoView : public CompositeView {
 public:
  UnitInfoView(const Rect*, core::Unit* = NULL);
  virtual ~UnitInfoView();
  void SetUnitTerrainInfo(core::Cell* cell);
  void SetUnitAttackInfo(core::Unit*, int, int);
  void SetUnit(core::Unit*);
  void SetContents(const std::string& name, int lv, const core::HpMp& hpmp_cur, const core::HpMp& hpmp_max, const core::HpMp& hpmp_ext);

  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent) override;

 private:
  core::Unit*      unit_;
  GaugeView* gv_hp_;
  GaugeView* gv_mp_;
  TextView*  tv_name_;
  TextView*  tv_lv_;
  TextView*  tv_lftbot_;
  TextView*  tv_rgtbot_;
};

} // namespace app
} // namespace gui
} // namespace mengde

#endif

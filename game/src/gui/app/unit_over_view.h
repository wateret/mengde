#ifndef UNIT_OVER_VIEW_H_
#define UNIT_OVER_VIEW_H_

#include "gui/uifw/composite_view.h"

#include "common.h"

namespace mengde {
namespace core {
class Unit;
}
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class UnitOverView : public CompositeView {
 public:
  UnitOverView(const Rect&);
  void SetUnit(core::Unit* unit);

 private:
  void OnUnitUpdate();

 private:
  static const int kNumExp   = 1;
  static const int kNumHpMp  = 2;
  static const int kNumStats = 5;
  static const int kNumGVs   = kNumExp + kNumHpMp + kNumStats;
  static const int kNumTabs  = 3;

 private:
  core::Unit* unit_;
  ImageView*  iv_portrait_;
  TextView*   tv_name_;
  TextView*   tv_lv_;
  TextView*   tv_stats_[kNumGVs];
  GaugeView*  gv_stats_[kNumGVs];
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // UNIT_OVER_VIEW_H_

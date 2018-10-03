#ifndef MENGDE_GUI_APP_UNIT_STATUS_VIEW_H_
#define MENGDE_GUI_APP_UNIT_STATUS_VIEW_H_

#include "gui/uifw/vertical_list_view.h"
#include "common.h"

namespace mengde { namespace core { class IUnitBase; } }

namespace mengde {
namespace gui {
namespace app {

class UnitStatusView : public uifw::VerticalListView {
 public:
  UnitStatusView(const Rect& frame);

 public:
  void SetUnit(const core::IUnitBase* unit);

 private:
  void OnUnitUpdate();

 private:
  const core::IUnitBase* unit_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_UNIT_STATUS_VIEW_H_

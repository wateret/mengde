#include "unit_status_view.h"

#include "core/unit.h"
#include "gui/uifw/text_view.h"

namespace mengde {
namespace gui {
namespace app {

UnitStatusView::UnitStatusView(const Rect& frame) : VerticalListView{frame}, unit_{NULL} {
  bg_color(COLOR("darkgray"));
}

void UnitStatusView::SetUnit(const core::IUnitBase* unit) {
  unit_ = unit;
  OnUnitUpdate();
}

void UnitStatusView::OnUnitUpdate() {
  ASSERT(unit_ != NULL);

  RemoveAllChildren();
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

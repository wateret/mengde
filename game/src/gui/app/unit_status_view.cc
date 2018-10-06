#include "unit_status_view.h"

#include "core/stat_modifier.h"
#include "core/unit.h"
#include "gui/uifw/text_view.h"

namespace mengde {
namespace gui {
namespace app {

UnitStatusView::UnitStatusView(const Rect& frame) : VerticalListView{frame}, unit_{NULL} {
  bg_color(COLOR("darkgray"));
}

void UnitStatusView::SetUnit(const core::Unit* unit) {
  unit_ = unit;
  OnUnitUpdate();
}

void UnitStatusView::OnUnitUpdate() {
  ASSERT(unit_ != NULL);

  RemoveAllChildren();

  unit_->volatile_attribute().stat_modifier_list().iterate([&](const core::StatModifier& mod) {
    AddElement(new TextView{Rect{0, 0, 220, 30}, mod.ToString()});
  });
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

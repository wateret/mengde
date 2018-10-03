#include "unit_view.h"

#include "core/i_unit_base.h"
#include "equipment_set_view.h"
#include "unit_over_view.h"
#include "unit_status_view.h"

namespace mengde {
namespace gui {
namespace app {

UnitView::UnitView(const Rect& frame) : TabView(frame), unit_(NULL) {
  Vec2D size = frame.size();
  size -= {0, -28};
  Rect subframe({0, 28}, size);
  unit_over_view_ = new UnitOverView(subframe);
  equipment_set_view_ = new EquipmentSetView(subframe);
  unit_status_view_ = new UnitStatusView(subframe);

  // TODO Hardcoded tab button name
  AddTab("Stat", unit_over_view_);
  AddTab("Equip", equipment_set_view_);
  AddTab("Status", unit_status_view_);
  //  SetViewIndex(1);
}

void UnitView::SetUnit(const core::IUnitBase* unit) {
  if (unit_ != unit) {
    unit_ = unit;
    unit_over_view_->SetUnit(unit);
    equipment_set_view_->SetEquipmentSet(unit->GetEquipmentSet());
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

#include "unit_status_view.h"

#include "core/event_effect.h"
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

  Clear();

  const Rect element_rect{0, 0, 220, 20};

  auto weapon = unit_->GetEquipmentSet()->GetWeapon();
  auto armor = unit_->GetEquipmentSet()->GetArmor();
  auto aid = unit_->GetEquipmentSet()->GetAid();

  {
    auto sm_handler = [&](const core::StatModifier& mod) { AddElement(new TextView{element_rect, mod.ToString()}); };

    unit_->volatile_attribute().stat_modifier_list().iterate(sm_handler);
    if (weapon) weapon->volatile_attribute().stat_modifier_list().iterate(sm_handler);
    if (armor) armor->volatile_attribute().stat_modifier_list().iterate(sm_handler);
    if (aid) aid->volatile_attribute().stat_modifier_list().iterate(sm_handler);
  }

  {
    auto ee_handler = [&](const core::EventEffectBase& eff) {
      auto turns_left = std::to_string(eff.GetTurnsLeft()) + "T";
      AddElement(new TextView{element_rect, "[some_effect] " + turns_left});
    };

    unit_->volatile_attribute().event_effect_list().iterate(ee_handler);
    if (weapon) weapon->volatile_attribute().event_effect_list().iterate(ee_handler);
    if (armor) armor->volatile_attribute().event_effect_list().iterate(ee_handler);
    if (aid) aid->volatile_attribute().event_effect_list().iterate(ee_handler);
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

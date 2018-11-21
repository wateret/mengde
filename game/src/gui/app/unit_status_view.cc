#include "unit_status_view.h"

#include "core/event_effect.h"
#include "core/stat_modifier.h"
#include "core/unit.h"
#include "gui/uifw/text_view.h"

namespace mengde {
namespace gui {
namespace app {

UnitStatusView::UnitStatusView(const Rect& frame) : TableView{frame}, unit_{NULL} {}

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

  AddCol("name");
  AddCol("turn");
  AddCol("addend");
  AddCol("mul");
  {
    auto sm_handler = [&](const core::StatModifier& mod) {
      auto id = string(core::Attribute::kToString[mod.stat_id()]);
      auto turns_left = std::to_string(mod.turn().left());
      auto addend = std::to_string(mod.addend());
      auto mul = std::to_string(mod.multiplier());

      AddRow({new TextView{element_rect, id}, new TextView{element_rect, turns_left + "T"},
              new TextView{element_rect, addend + "+"}, new TextView{element_rect, mul + "%"}});
    };

    unit_->volatile_attribute().stat_modifier_list().iterate(sm_handler);
    if (weapon) weapon->volatile_attribute().stat_modifier_list().iterate(sm_handler);
    if (armor) armor->volatile_attribute().stat_modifier_list().iterate(sm_handler);
    if (aid) aid->volatile_attribute().stat_modifier_list().iterate(sm_handler);
  }

  {
    auto ee_handler = [&](const core::EventEffectBase& eff) {
      auto turns_left = std::to_string(eff.turn().left()) + "T";
      AddRow({new TextView{element_rect, "[some_effect] "}, new TextView{element_rect, turns_left}});
    };

    unit_->volatile_attribute().event_effect_list().iterate(ee_handler);
    if (weapon) weapon->volatile_attribute().event_effect_list().iterate(ee_handler);
    if (armor) armor->volatile_attribute().event_effect_list().iterate(ee_handler);
    if (aid) aid->volatile_attribute().event_effect_list().iterate(ee_handler);
  }

  {
    auto cond_handler = [&](core::Condition condition, core::TurnBased turns) {
      auto cond_name = [](core::Condition c) -> std::string {
        if (c == core::Condition::kStunned) return "stunned";
        if (c == core::Condition::kRooted) return "rooted";
        if (c == core::Condition::kPoisoned) return "poisoned";
        return "";
      }(condition);
      auto turns_left = std::to_string(turns.left()) + "T";
      AddRow({new TextView{element_rect, cond_name}, new TextView{element_rect, turns_left}});
    };

    unit_->condition_set().Iterate(cond_handler);
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

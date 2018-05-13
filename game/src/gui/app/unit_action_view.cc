#include "unit_action_view.h"

#include "core/cmd.h"
#include "core/game.h"
#include "game_view.h"
#include "gui/uifw/button_view.h"
#include "state_ui.h"

namespace mengde {
namespace gui {
namespace app {

UnitActionView::UnitActionView(const Rect& frame, core::Game* game, core::UserInterface* gi, GameView* gv)
    : VerticalListView(frame), game_(game), gi_(gi), gv_(gv) {
  bg_color(COLOR("darkgray", 212));
  padding(8);
  Rect btn_frame = GetActualFrame();
  btn_frame.SetH(24);

  btn_attack_ = new ButtonView(&btn_frame, "Attack");
  AddElement(btn_attack_);
  SetMargin(4);
  btn_magic_ = new ButtonView(&btn_frame, "Magic");
  AddElement(btn_magic_);
  btn_stay_ = new ButtonView(&btn_frame, "Stay");
  AddElement(btn_stay_);
}

void UnitActionView::SetUnitAndMoveId(uint32_t unit_id, uint32_t move_id) {
  auto game = game_;
  auto gi   = gi_;
  auto gv   = gv_;

  btn_attack_->SetMouseButtonHandler([=](const foundation::MouseButtonEvent& e) {
    if (e.IsLeftButtonUp()) {
      gv_->PushUIState(new StateUITargeting({game, gi, gv}, unit_id, move_id));
    }
    return true;
  });
  btn_magic_->SetMouseButtonHandler([=](const foundation::MouseButtonEvent& e) {
    if (e.IsLeftButtonUp()) {
      gv_->PushUIState(new StateUIMagicSelection({game, gi, gv}, unit_id, move_id));
    }
    return true;
  });
  btn_stay_->SetMouseButtonHandler([=](const foundation::MouseButtonEvent& e) {
    if (e.IsLeftButtonUp()) {
      gi_->PushAction(unit_id, move_id, core::ActionType::kStay, 0);
      gv_->InitUIStateMachine();
    }
    return true;
  });
}

bool UnitActionView::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  // Refuse to handle RightButtonUp
  if (e.IsRightButtonUp()) {
    return false;
  }
  return CompositeView::OnMouseButtonEvent(e);
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

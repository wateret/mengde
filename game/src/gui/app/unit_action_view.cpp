#include "unit_action_view.h"

#include "root_view.h"
#include "state_ui.h"
#include "gui/uifw/button_view.h"
#include "core/cmd.h"
#include "core/game.h"

UnitActionView::UnitActionView(const Rect& frame, mengde::core::Game* game, RootView* rv)
    : VerticalListView(frame), game_(game), rv_(rv) {
  bg_color(COLOR("darkgray", 160));
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

void UnitActionView::SetUnit(mengde::core::Unit* unit) {
  btn_attack_->SetMouseButtonHandler([=] (const MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      rv_->PushUIState(new StateUITargeting({game_, rv_}, unit));
    }
    return true;
  });
  btn_magic_->SetMouseButtonHandler([=] (const MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      rv_->PushUIState(new StateUIMagicSelection({game_, rv_}, unit));
    }
    return true;
  });
  btn_stay_->SetMouseButtonHandler([=] (const MouseButtonEvent e) {
    unique_ptr<mengde::core::CmdAction> action(new mengde::core::CmdAction());
    action->SetCmdMove(unique_ptr<mengde::core::CmdMove>(new mengde::core::CmdMove(unit, unit->GetPosition())));
    action->SetCmdAct(unique_ptr<mengde::core::CmdStay>(new mengde::core::CmdStay(unit)));
    game_->PushCmd(std::move(action));
    rv_->InitUIStateMachine();
    return true;
  });
}

bool UnitActionView::OnMouseButtonEvent(const MouseButtonEvent e) {
  // Refuse to handle RightButtonUp
  if (e.IsRightButtonUp()) {
    return false;
  }
  return CompositeView::OnMouseButtonEvent(e);
}

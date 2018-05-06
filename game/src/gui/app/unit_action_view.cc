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

void UnitActionView::SetUnit(core::Unit* unit) {
  btn_attack_->SetMouseButtonHandler([=](const foundation::MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      gv_->PushUIState(new StateUITargeting({game_, gi_, gv_}, unit));
    }
    return true;
  });
  btn_magic_->SetMouseButtonHandler([=](const foundation::MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      gv_->PushUIState(new StateUIMagicSelection({game_, gi_, gv_}, unit));
    }
    return true;
  });
  btn_stay_->SetMouseButtonHandler([=](const foundation::MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      unique_ptr<core::CmdAction> action(new core::CmdAction());
      action->SetCmdMove(unique_ptr<core::CmdMove>(new core::CmdMove(unit, unit->GetPosition())));
      action->SetCmdAct(unique_ptr<core::CmdStay>(new core::CmdStay(unit)));
      game_->Push(std::move(action));
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

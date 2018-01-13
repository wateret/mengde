#include "unit_action_view.h"

#include "root_view.h"
#include "state_ui.h"
#include "view/uifw/button_view.h"

UnitActionView::UnitActionView(const Rect& frame, mengde::core::Game* game, RootView* rv) : CompositeView(frame), game_(game), rv_(rv) {
  SetPadding(8);
  SetBgColor(COLOR("darkgray", 160));
  Rect btn_frame = GetActualFrame();
  btn_frame.SetH(24);

  btn_attack_ = new ButtonView(&btn_frame, "Attack");
  btn_frame.SetY(btn_frame.GetY() + btn_frame.GetH());
  AddChild(btn_attack_);

  btn_magic_ = new ButtonView(&btn_frame, "Magic");
  btn_frame.SetY(btn_frame.GetY() + btn_frame.GetH());
  AddChild(btn_magic_);
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
}

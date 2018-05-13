#include "main_view.h"
#include "gui/app/app.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/layout_helper.h"
#include "scenario_select_view.h"

namespace mengde {
namespace gui {
namespace app {

MainView::MainView(const Rect* frame, App* app) : CompositeView(frame) {
  bg_color(COLOR("white"));
  const Vec2D kButtonSize = {200, 100};
  Rect start_frame = LayoutHelper::CalcPosition(frame, kButtonSize, LayoutHelper::kAlignHMid);
  start_frame.Move(0, 100);
  Rect quit_frame = start_frame;
  quit_frame.Move(0, 150);
  ButtonView* start_button = new ButtonView(&start_frame, "New Game");
  ButtonView* quit_button = new ButtonView(&quit_frame, "Quit");
  start_button->SetMouseButtonHandler([this](const foundation::MouseButtonEvent& e) -> bool {
    if (e.IsLeftButtonUp()) {
      LOG_DEBUG("Start");
      this->SetScenarioSelectViewVisible(true);
    }
    return true;
  });
  quit_button->SetMouseButtonHandler([app](const foundation::MouseButtonEvent& e) -> bool {
    if (e.IsLeftButtonUp()) {
      app->SetQuit(true);
    }
    return true;
  });
  AddChild(start_button);
  AddChild(quit_button);

  Rect scenario_select_frame(start_frame.GetRight() + 8, start_frame.GetH(), 240, 300);
  scenario_select_view_ = new ScenarioSelectView(scenario_select_frame, app);
  scenario_select_view_->visible(false);

  AddChild(scenario_select_view_);
}

void MainView::SetScenarioSelectViewVisible(bool b) { scenario_select_view_->visible(b); }

}  // namespace app
}  // namespace gui
}  // namespace mengde

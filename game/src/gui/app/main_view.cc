#include "main_view.h"
#include "gui/app/app.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/layout_helper.h"
#include "savefile_select_view.h"
#include "scenario_select_view.h"

namespace mengde {
namespace gui {
namespace app {

MainView::MainView(const Rect& frame, App* app) : CompositeView(frame) {
  bg_color(COLOR("white"));
  const Vec2D kButtonSize = {200, 80};
  Rect start_frame = LayoutHelper::CalcPosition(frame, kButtonSize, LayoutHelper::kAlignHMid);
  start_frame.Move(0, 100);
  Rect load_frame = start_frame;
  load_frame.Move(0, 120);
  Rect quit_frame = load_frame;
  quit_frame.Move(0, 120);
  ButtonView* start_button = new ButtonView(&start_frame, "New Game");
  ButtonView* load_button = new ButtonView(&load_frame, "Load Game");
  ButtonView* quit_button = new ButtonView(&quit_frame, "Quit");
  start_button->SetMouseButtonHandler([this](const foundation::MouseButtonEvent& e) -> bool {
    if (e.IsLeftButtonUp()) {
      LOG_DEBUG("Start");
      this->scenario_select_view_->visible(true);
      this->savefile_select_view_->visible(false);
    }
    return true;
  });
  load_button->SetMouseButtonHandler([this](const foundation::MouseButtonEvent& e) -> bool {
    if (e.IsLeftButtonUp()) {
      this->savefile_select_view_->visible(true);
      this->scenario_select_view_->visible(false);
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
  AddChild(load_button);
  AddChild(quit_button);

  Rect scenario_select_frame(start_frame.right() + 8, start_frame.y(), 240, 500);
  scenario_select_view_ = new ScenarioSelectView(scenario_select_frame, app);
  scenario_select_view_->visible(false);

  Rect savefile_select_frame(load_frame.right() + 8, load_frame.y(), 240, 500);
  savefile_select_view_ = new SavefileSelectView(savefile_select_frame, app);
  savefile_select_view_->visible(false);

  AddChild(scenario_select_view_);
  AddChild(savefile_select_view_);
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

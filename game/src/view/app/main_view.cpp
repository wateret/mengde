#include "main_view.h"
#include "view/app/app.h"
#include "view/foundation/layout_helper.h"
#include "view/uifw/button_view.h"

MainView::MainView(const Rect* frame, App* app) : CompositeView(frame) {
  bg_color(COLOR("white"));
  const Vec2D kButtonSize = {100, 100};
  Rect start_frame = LayoutHelper::CalcPosition(frame, {100, 100}, LayoutHelper::kAlignHMid);
  start_frame.Move(0, 100);
  Rect quit_frame = start_frame;
  quit_frame.Move(0, 150);
  ButtonView* start_button = new ButtonView(&start_frame, "Start");
  ButtonView* quit_button = new ButtonView(&quit_frame, "Quit");
  start_button->SetMouseButtonHandler([app] (const MouseButtonEvent e) -> bool {
    if (e.IsLeftButtonUp()) {
      LOG_DEBUG("Start");
      app->StartNewGame();
    }
    return true;
  });
  quit_button->SetMouseButtonHandler([app] (const MouseButtonEvent e) -> bool {
    if (e.IsLeftButtonUp()) {
      app->SetQuit(true);
    }
    return true;
  });
  AddChild(start_button);
  AddChild(quit_button);
}

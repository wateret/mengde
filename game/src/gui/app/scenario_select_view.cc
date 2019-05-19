#include "scenario_select_view.h"

#include <boost/filesystem.hpp>

#include "app.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/text_view.h"

namespace mengde {
namespace gui {
namespace app {

ScenarioSelectView::ScenarioSelectView(const Rect& frame, App* app) : VerticalListView(frame) {
  bg_color(COLOR("black"));
  padding(8);

  // Add header text
  const Rect header_frame({0, 0}, {GetActualFrameSize().x, 32});
  TextView* header = new TextView(header_frame, "Choose Scenario", COLOR("yellow"), 16, LayoutHelper::kAlignCenter);
  AddElement(header);

  // Add scenario entries
  // TODO Move filesystem works to `util` module?
  boost::filesystem::path sce_path("sce");
  if (boost::filesystem::exists(sce_path) && boost::filesystem::is_directory(sce_path)) {
    for (boost::filesystem::directory_iterator itr(sce_path); itr != boost::filesystem::directory_iterator(); itr++) {
      // TODO Check if it contains mandatory files like config.lua
      const Vec2D btn_size = {GetActualFrameSize().x, 24};
      if (boost::filesystem::is_directory(*itr)) {
        const string scenario_id = itr->path().filename().string();
        ButtonView* btn_element = new ButtonView(Rect({0, 0}, btn_size), scenario_id);
        btn_element->SetMouseButtonHandler([=](const MouseButtonEvent& e) {
          if (e.IsLeftButtonDown()) {
            visible(false);
            app->StartNewScenario(scenario_id);
            return true;
          }
          return false;
        });
        AddElement(btn_element);
      }
    }
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

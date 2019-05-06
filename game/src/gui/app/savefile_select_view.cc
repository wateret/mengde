#include "savefile_select_view.h"

#include <boost/filesystem.hpp>

#include "app.h"
#include "core/deserializer.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/text_view.h"

namespace mengde {
namespace gui {
namespace app {

SavefileSelectView::SavefileSelectView(const Rect& frame, App* /* app */) : VerticalListView(frame) {
  bg_color(COLOR("black"));
  padding(8);

  // Add header text
  const Rect header_frame({0, 0}, {GetActualFrameSize().x, 32});
  TextView* header = new TextView(header_frame, "Choose Savefile", COLOR("yellow"), 16, LayoutHelper::kAlignCenter);
  AddElement(header);

  // Scan and add save files to the list
  boost::filesystem::path sav_path("sav");
  if (boost::filesystem::exists(sav_path) && boost::filesystem::is_directory(sav_path)) {
    for (boost::filesystem::directory_iterator itr(sav_path); itr != boost::filesystem::directory_iterator(); ++itr) {
      Vec2D btn_pos{0, 0};
      if (!boost::filesystem::is_directory(*itr) && itr->path().filename().extension() == ".mengde") {
        const Vec2D btn_size = {GetActualFrameSize().x, 24};
        const string scenario_id = itr->path().stem().string();
        ButtonView* btn_element = new ButtonView(Rect(btn_pos, btn_size), scenario_id);
        auto path = itr->path().string();
        btn_element->SetMouseButtonHandler([=](const MouseButtonEvent& e) {
          if (e.IsLeftButtonDown()) {
            core::Deserializer deserializer{path};
            deserializer.Deserialize();
            return true;
          }
          return false;
        });
        AddElement(btn_element);
        btn_pos.y += btn_size.y + 4;
      }
    }
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

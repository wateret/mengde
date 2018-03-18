#ifndef GUI_APP_SCENARIO_SELECT_VIEW_H_
#define GUI_APP_SCENARIO_SELECT_VIEW_H_

#include "common.h"
#include "gui/uifw/vertical_list_view.h"

namespace mengde {
namespace gui {
namespace app {

class App;

class ScenarioSelectView : public VerticalListView {
 public:
  ScenarioSelectView(const Rect& frame, App* app);

 private:
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // GUI_APP_SCENARIO_SELECT_VIEW_H_

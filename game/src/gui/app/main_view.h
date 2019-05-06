#ifndef MENGDE_GUI_APP_MAIN_VIEW_H_
#define MENGDE_GUI_APP_MAIN_VIEW_H_

#include "gui/uifw/composite_view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace app {

class App;
class SavefileSelectView;
class ScenarioSelectView;

class MainView : public CompositeView {
 public:
  MainView(const Rect&, App*);
  void SetScenarioSelectViewVisible(bool b);

 private:
  ScenarioSelectView* scenario_select_view_;
  SavefileSelectView* savefile_select_view_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_MAIN_VIEW_H_

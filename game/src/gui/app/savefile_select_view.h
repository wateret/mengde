#ifndef MENGDE_GUI_APP_SAVEFILE_SELECT_VIEW_H_
#define MENGDE_GUI_APP_SAVEFILE_SELECT_VIEW_H_

#include "common.h"
#include "gui/uifw/vertical_list_view.h"

namespace mengde {
namespace gui {
namespace app {

class App;

class SavefileSelectView : public VerticalListView {
 public:
  SavefileSelectView(const Rect& frame, App* app);

 private:
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_SAVEFILE_SELECT_VIEW_H_

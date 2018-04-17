#ifndef MENGDE_GUI_APP_CONTROL_VIEW_H_
#define MENGDE_GUI_APP_CONTROL_VIEW_H_

#include "gui/uifw/composite_view.h"

#include "common.h"

namespace mengde {
namespace core {
class Game;
}
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class GameView;

class ControlView : public CompositeView {
 public:
  ControlView(const Rect*, core::Game*, GameView*);
  void SetTurnText(int cur_turn, int max_turn);
  void SetEndTurnVisible(bool);

 private:
  core::Game* game_;
  TextView*   tv_turn_;
  ButtonView* btn_end_turn_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_CONTROL_VIEW_H_

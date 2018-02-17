#ifndef CONTROL_VIEW_H_
#define CONTROL_VIEW_H_

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

class RootView;

class ControlView : public CompositeView {
 public:
  ControlView(const Rect*, core::Game*, RootView*);
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

#endif  // CONTROL_VIEW_H_

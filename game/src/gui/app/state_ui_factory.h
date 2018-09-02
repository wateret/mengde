#ifndef MENGDE_GUI_APP_STATE_UI_FACTORY_H_
#define MENGDE_GUI_APP_STATE_UI_FACTORY_H_

#include "state_ui.h"

namespace mengde {
namespace gui {
namespace app {

class StateUIFactory {
 public:
  StateUIFactory(core::UserInterface* gi, GameView* gv) : gi_{gi}, gv_{gv} {}

 public:
  StateUI* GenerateNext();

 private:
  StateUI::Base WrapBase() { return {gi_, gv_}; }

 private:
  core::UserInterface* gi_;
  GameView* gv_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_STATE_UI_FACTORY_H_

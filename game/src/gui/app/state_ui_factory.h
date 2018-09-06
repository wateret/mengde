#ifndef MENGDE_GUI_APP_STATE_UI_FACTORY_H_
#define MENGDE_GUI_APP_STATE_UI_FACTORY_H_

#include "state_ui.h"
#include "core/cmd_visitor.h"

namespace mengde {
namespace gui {
namespace app {

class StateUIGenerator : public mengde::core::CmdVisitor {
 public:
  StateUIGenerator(mengde::core::UserInterface* gi, mengde::gui::app::GameView* gv) : gi_{gi}, gv_{gv} {}

 public:
#define MACRO_CMD_OP(name) virtual void Visit(const mengde::core::Cmd##name& cmd) override final;
#include "core/cmd_op.h.inc"

 public:
  mengde::gui::app::StateUI* TakeGenerated() { return generated_; }

 private:
  mengde::gui::app::StateUI::Base WrapBase() { return {gi_, gv_}; }

 private:
  mengde::core::UserInterface* gi_;
  mengde::gui::app::GameView* gv_;
  mengde::gui::app::StateUI* generated_{nullptr};
};

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

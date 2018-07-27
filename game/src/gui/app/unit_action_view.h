#ifndef MENGDE_GUI_APP_UNIT_ACTION_VIEW_H_
#define MENGDE_GUI_APP_UNIT_ACTION_VIEW_H_

#include <boost/optional.hpp>

#include "core/id.h"
#include "gui/uifw/vertical_list_view.h"

#include "common.h"

namespace mengde {
namespace core {
class Game;
class UserInterface;
class Unit;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class GameView;

class UnitActionView : public VerticalListView {
 public:
  UnitActionView(const Rect& frame, core::Game* game, core::UserInterface*, GameView* gv);
  void SetUnitAndMoveId(uint32_t unit_id, uint32_t move_id);
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;

 private:
  core::Game* game_;  // TODO remove this
  core::UserInterface* gi_;
  GameView* gv_;
  ButtonView* btn_attack_;
  ButtonView* btn_magic_;
  ButtonView* btn_stay_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_UNIT_ACTION_VIEW_H_

#ifndef UNIT_ACTION_VIEW_H_
#define UNIT_ACTION_VIEW_H_

#include "gui/uifw/vertical_list_view.h"

#include "common.h"

namespace mengde {
namespace core {
class Game;
class Unit;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class RootView;

class UnitActionView : public VerticalListView {
 public:
  UnitActionView(const Rect& frame, core::Game* game, RootView* rv);
  void         SetUnit(core::Unit* unit);
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override;

 private:
  core::Game* game_;
  RootView*   rv_;
  ButtonView* btn_attack_;
  ButtonView* btn_magic_;
  ButtonView* btn_stay_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // UNIT_ACTION_VIEW_H_

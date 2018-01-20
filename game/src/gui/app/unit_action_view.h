#ifndef UNIT_ACTION_VIEW_H_
#define UNIT_ACTION_VIEW_H_

#include "gui/uifw/vertical_list_view.h"

namespace mengde {
namespace core {
  class Game;
  class Unit;
}
}

class ButtonView;
class RootView;

class UnitActionView : public VerticalListView {
 public:
  UnitActionView(const Rect& frame, mengde::core::Game* game, RootView* rv);
  void SetUnit(mengde::core::Unit* unit);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;

 private:
  mengde::core::Game* game_;
  RootView*   rv_;
  ButtonView* btn_attack_;
  ButtonView* btn_magic_;
  ButtonView* btn_stay_;
};

#endif // UNIT_ACTION_VIEW_H_

#ifndef MENGDE_GUI_APP_ROOT_VIEW_H_
#define MENGDE_GUI_APP_ROOT_VIEW_H_

#include "common.h"
#include "gui/uifw/view.h"
#include "util/common.h"

namespace mengde {
namespace core {

class Assets;
class Game;
class Unit;
class Scenario;
struct HpMp;

}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class App;

class DeployView;
class GameView;
class UIViews;

class RootView : public View {
 public:
  RootView(const Rect& frame, core::Scenario*, App*);
  ~RootView();

  // Overriden method (View)
  virtual void Update() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent) override;

  // UI view getters
  UIViews* ui_views() { return ui_views_; }

  void RaiseMouseOverEvent();

 private:
  App* app_;

  GameView* game_view_;
  UIViews*  ui_views_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_ROOT_VIEW_H_

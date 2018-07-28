#ifndef MENGDE_GUI_APP_GAME_VIEW_H_
#define MENGDE_GUI_APP_GAME_VIEW_H_

#include <unordered_set>

#include "common.h"
#include "core/id.h"
#include "gui/uifw/view.h"
#include "state_ui.h"
#include "ui_views.h"  // FIXME Temporary include
#include "util/state_machine.h"

namespace mengde {
namespace core {

class Assets;
class Stage;
class Scenario;
class UserInterface;
struct HpMp;

}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class App;
class DeployView;
class UnitTooltipView;
class UnitView;
class ControlView;
class UnitDialogView;
class MagicListView;
class TerrainInfoView;
class UnitListView;
class UnitActionView;

class UIViews;

class GameView : public View {
 public:
  typedef function<void()> NextFrameCallback;

 public:
  GameView(const Rect& frame, core::Stage* game, App* app);
  ~GameView();

 public:
  // View interfaces
  virtual void Update() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent&) override;
  virtual bool OnKeyEvent(const foundation::KeyEvent&) override;

 public:
  // UIStateMachine related
  void ChangeUIState(StateUI*);
  void PushUIState(StateUI*);
  void PopUIState();
  void InitUIStateMachine();
  StateUI* GetCurrentState() { return ui_state_machine_.GetCurrentState(); }

 public:
  // Camera related
  Vec2D GetCameraCoords() { return camera_coords_; }
  Vec2D* GetCameraCoordsPtr() { return &camera_coords_; }
  Vec2D GetMaxCameraCoords() { return max_camera_coords_; }
  void SetMaxCameraCoords(Vec2D c) { max_camera_coords_ = c; }
  void MoveCameraX(int d);
  void MoveCameraY(int d);
  void CenterCamera(Vec2D);

 public:
  TerrainInfoView* terrain_info_view() { return ui_views_->terrain_info_view(); }
  UnitListView* unit_list_view() { return ui_views_->unit_list_view(); }
  UnitDialogView* unit_dialog_view() { return ui_views_->unit_dialog_view(); }
  ModalView* unit_dialog_view_wrapper() { return ui_views_->unit_dialog_view_wrapper(); }
  UnitView* unit_view() { return ui_views_->unit_view(); }
  UnitTooltipView* unit_tooltip_view() { return ui_views_->unit_tooltip_view(); }
  ControlView* control_view() { return ui_views_->control_view(); }
  ModalDialogView* dialog_view() { return ui_views_->dialog_view(); }
  MagicListView* magic_list_view() { return ui_views_->magic_list_view(); }
  UnitActionView* unit_action_view() { return ui_views_->unit_action_view(); }

 public:
  Vec2D GetMouseCoords() { return mouse_coords_; }
  void EndStage();
  void NextFrame(NextFrameCallback);
  void SetUIViews(UIViews* ui_views) { ui_views_ = ui_views; }
  void RaiseMouseOverEvent();
  void SetSkipRender(const core::UId& id, bool b);
  void RenderUnit(Drawer* drawer, const core::Unit* unit, Vec2D pos);

 private:
  void RunCallbacks();
  int GetCurrentSpriteNo(int, int) const;
  bool SkipRender(const core::UId& id) const;

 private:
  core::Stage* stage_;  // TODO We should eventually remove this, use core::UserInterface instead.
  core::UserInterface* gi_;

  App* app_;

  StateMachine<StateUI*> ui_state_machine_;
  queue<NextFrameCallback> frame_callbacks_;

  std::unordered_set<uint32_t> skip_render_;

  Vec2D mouse_coords_;
  Vec2D camera_coords_;
  Vec2D max_camera_coords_;
  int frame_count_;

  UIViews* ui_views_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_GAME_VIEW_H_

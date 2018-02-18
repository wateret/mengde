#ifndef ROOT_VIEW_H_
#define ROOT_VIEW_H_

#include "gui/uifw/view.h"
#include "util/common.h"
#include "util/state_machine.h"

#include "common.h"

namespace mengde {
namespace core {

class Assets;
class Game;
class Unit;
class Cell;
class Scenario;
struct HpMp;

}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class App;
class StateUI;

class DeployView;
// class EquipmentSelectView;
class UnitView;
class UnitDialogView;
class ControlView;
class MagicListView;
class UnitActionView;
class UnitInfoView;
class UnitListView;
class TerrainInfoView;

class RootView : public View {
 public:
  typedef function<void()> NextFrameCallback;

 public:
  RootView(const Vec2D, core::Scenario*, App*);
  ~RootView();
  Vec2D  GetMouseCoords() { return mouse_coords_; }
  Vec2D  GetCameraCoords() { return camera_coords_; }
  Vec2D* GetCameraCoordsPtr() { return &camera_coords_; }
  Vec2D  GetMaxCameraCoords() { return max_camera_coords_; }
  void   SetMaxCameraCoords(Vec2D c) { max_camera_coords_ = c; }
  void   MoveCameraX(int d);
  void   MoveCameraY(int d);

  TerrainInfoView* terrain_info_view() { return terrain_info_view_; }
  UnitListView*    unit_list_view() { return unit_list_view_; }
  UnitDialogView*  unit_dialog_view() { return unit_dialog_view_; }
  UnitView*        unit_view() { return unit_view_; }
  UnitInfoView*    unit_info_view() { return unit_info_view_; }
  ControlView*     control_view() { return control_view_; }
  ModalDialogView* dialog_view() { return dialog_view_; }
  MagicListView*   magic_list_view() { return magic_list_view_; }
  UnitActionView*  unit_action_view() { return unit_action_view_; }

  void CenterCamera(Vec2D);
  void EndGame();
  void NextFrame(NextFrameCallback);

  // UIStateMachine related
  void     ChangeUIState(StateUI*);
  void     PushUIState(StateUI*);
  void     PopUIState();
  void     InitUIStateMachine();
  StateUI* GetCurrentState() { return ui_state_machine_.GetCurrentState(); }

  // Overriden method (View)
  virtual void Update() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent) override;

 private:
  void RunCallbacks();
  int  GetCurrentSpriteNo(int, int) const;
  void RaiseMouseMotionEvent();

 private:
  core::Game* game_;
  App*        app_;

  CompositeView*   ui_views_;
  DeployView*      deploy_view_;
  UnitInfoView*    unit_info_view_;
  UnitView*        unit_view_;
  ControlView*     control_view_;
  ModalDialogView* dialog_view_;
  UnitDialogView*  unit_dialog_view_;
  ModalView*       unit_dialog_view_wrapper_;
  MagicListView*   magic_list_view_;
  TerrainInfoView* terrain_info_view_;
  UnitListView*    unit_list_view_;
  UnitActionView*  unit_action_view_;

  StateMachine<StateUI*> ui_state_machine_;

  queue<NextFrameCallback> reserved_callbacks_;

  Vec2D mouse_coords_;
  Vec2D camera_coords_;
  Vec2D max_camera_coords_;
  int   frame_count_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // ROOT_VIEW_H_

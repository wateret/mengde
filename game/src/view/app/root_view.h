#ifndef ROOT_VIEW_H_
#define ROOT_VIEW_H_

#include "view/foundation/view.h"
#include "util/common.h"
#include "util/state_machine.h"

namespace mengde {
namespace core {
  class Assets;
  class Game;
  class Unit;
  class Cell;
  class Scenario;
  struct HpMp;
}
}

class App;
class StateUI;

class CompositeView;
class DeployView;
class EquipmentSelectView;
class UnitView;
class UnitDialogView;
class ControlView;
class ModalDialogView;
class MagicListView;
class UnitActionView;
class UnitInfoView;
class UnitListView;
class TerrainInfoView;

class RootView : public View {
 public:
   typedef function<void()> NextFrameCallback;

 public:
  RootView(const Vec2D, mengde::core::Scenario*, App*);
  ~RootView();
  Vec2D GetMouseCoords() { return mouse_coords_; }
  Vec2D GetCameraCoords() { return camera_coords_; }
  Vec2D* GetCameraCoordsPtr() { return &camera_coords_; }
  Vec2D GetMaxCameraCoords() { return max_camera_coords_; }
  void SetMaxCameraCoords(Vec2D c) { max_camera_coords_ = c; }
  void MoveCameraX(int d);
  void MoveCameraY(int d);

  // View wrappers
  void SetControlViewTurnText(int, int);
  void SetUnitViewVisible(bool);
  void SetUnitViewUnit(mengde::core::Unit*);
  void SetUnitInfoViewUnitTerrainInfo(mengde::core::Cell* cell);
  void SetUnitInfoViewUnitAttackInfo(mengde::core::Unit* unit, int, int);
  void SetUnitInfoViewContents(const std::string&, int, const mengde::core::HpMp&, const mengde::core::HpMp&, int, int);
  void SetUnitInfoViewCoordsByUnitCoords(Vec2D, Vec2D);
  void SetUnitInfoViewVisible(bool);
  void SetTerrainInfoViewVisible(bool);
  void SetTerrainInfoViewText(const string&);
  void SetUnitListViewVisible(bool);
  void SetUnitListViewUnit(mengde::core::Unit* unit);
  UnitDialogView* unit_dialog_view() { return unit_dialog_view_; }
  ModalDialogView* dialog_view() { return dialog_view_; }
  MagicListView* magic_list_view() { return magic_list_view_; }
  UnitActionView* unit_action_view() { return unit_action_view_; }

  void CenterCamera(Vec2D);
  void EndGame();
  void NextFrame(NextFrameCallback);

  // UIStateMachine related
  void ChangeUIState(StateUI*);
  void PushUIState(StateUI*);
  void PopUIState();
  void InitUIStateMachine();
  StateUI* GetCurrentState() { return ui_state_machine_.GetCurrentState(); }

  // Overriden method (View)
  virtual void Update() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
  virtual bool OnMouseWheelEvent(const MouseWheelEvent) override;

 private:
  void RunCallbacks();
  int GetCurrentSpriteNo(int, int) const;
  void RaiseMouseMotionEvent();

 private:
  mengde::core::Game*                  game_;
  App*                   app_;
  CompositeView*         ui_views_;
  DeployView*            deploy_view_;
  UnitInfoView*          unit_info_view_;
  UnitView*              unit_view_;
  ControlView*           control_view_;
  ModalDialogView*       dialog_view_;
  UnitDialogView*        unit_dialog_view_;
  MagicListView*         magic_list_view_;
  TerrainInfoView*       terrain_info_view_;
  UnitListView*          unit_list_view_;
  EquipmentSelectView*   equipment_select_view_;
  UnitActionView*        unit_action_view_;
  StateMachine<StateUI*> ui_state_machine_;
  queue<NextFrameCallback> reserved_callbacks_;
  Vec2D mouse_coords_;
  Vec2D camera_coords_;
  Vec2D max_camera_coords_;
  int   frame_count_;
};

#endif // ROOT_VIEW_H_

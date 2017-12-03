#ifndef ROOT_VIEW_H_
#define ROOT_VIEW_H_

#include "view.h"
#include "common.h"
#include "state_machine.h"
#include "app.h"

class Game;
class StateUI;
class Unit;
class Cell;

class CompositeView;
class UnitView;
class UnitDialogView;
class ControlView;
class ModalDialogView;
class MagicListView;
class UnitInfoView;

class RootView : public View {
 public:
  RootView(const Vec2D, Game*, App*);
  ~RootView();
  Vec2D GetMouseCoords() { return mouse_coords_; }
  Vec2D GetCameraCoords() { return camera_coords_; }
  Vec2D* GetCameraCoordsPtr() { return &camera_coords_; }
  Vec2D GetMaxCameraCoords() { return max_camera_coords_; }
  void SetMaxCameraCoords(Vec2D c) { max_camera_coords_ = c; }
  void MoveCameraX(int d);
  void MoveCameraY(int d);
  void ShowMagicListView(Unit* unit);
  void HideMagicListView();
  void SetUnitViewVisible(bool);
  void SetUnitViewUnit(Unit*);
  void SetUnitInfoViewUnitTerrainInfo(Cell* cell);
  void SetUnitInfoViewVisible(bool);
  void SetDialogViewVisible(bool);
  void SetDialogViewText(const string&);
  void SetUnitDialogViewVisible(bool);
  bool IsUnitDialogViewVisible();
  void SetUnitDialogViewText(const string&);
  void SetUnitDialogViewUnit(Unit*);
  void CenterCamera(Vec2D);
  void EndGame();

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
  int GetCurrentSpriteNo(int, int) const;
  void RaiseMouseMotionEvent();

 private:
  Game*                  game_;
  App*                   app_;
  CompositeView*         ui_views_;
  UnitInfoView*          unit_info_view_;
  UnitView*              unit_view_;
  ControlView*           control_view_;
  ModalDialogView*       dialog_view_;
  UnitDialogView*        unit_dialog_view_;
  MagicListView*         magic_list_view_;
  StateMachine<StateUI*> ui_state_machine_;
  Vec2D mouse_coords_;
  Vec2D camera_coords_;
  Vec2D max_camera_coords_;
  int   frame_count_;
};

#endif // ROOT_VIEW_H_

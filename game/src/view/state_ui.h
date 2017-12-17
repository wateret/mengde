#ifndef STATE_UI_H_
#define STATE_UI_H_

#include "state.h"
#include "i_view.h"

// StateUI

class RootView;
class Game;
class Drawer;
class Unit;

class StateUI : public State, IView {
 public:
  struct Base {
    Game*     game;
    RootView* rv;
  };

 public:
  StateUI(Base);
  virtual ~StateUI() = default;
  virtual void Enter() override {}
  virtual void Exit() override {}
  virtual void Render(Drawer*) override {}
  virtual void Update() override {}
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override { return false; }
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override { return true; }
  virtual bool OnMouseWheelEvent(const MouseWheelEvent) override { return false; }
  Base WrapBase() { return {game_, rv_}; }

#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUI"; }
#endif

 protected:
  Game*     game_;
  RootView* rv_;
};

// StateUIMain

class StateUIMain : public StateUI {
 public:
  StateUIMain(Base);
//  virtual void Enter() override;
//  virtual void Exit() override;
//  virtual void Render(Drawer*) override;
//  virtual void Update() override;
//  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
//  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIMain"; }
#endif

};

// StateUIDoCmd

class StateUIDoCmd : public StateUI {
 public:
  StateUIDoCmd(Base);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIDoCmd"; }
#endif

 private:
  StateUI* GenerateNextCmdUIState();

 private:
  bool cmd_to_do_;
};

class StateUIOperable : public StateUI {
 public:
  StateUIOperable(StateUI::Base);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
  Vec2D GetCursorCell() { return cursor_cell_; }
  void ClearScrolls();
  void SetScrollLeft();
  void SetScrollRight();
  void SetScrollUp();
  void SetScrollDown();
  bool IsScrollLeft();
  bool IsScrollRight();
  bool IsScrollUp();
  bool IsScrollDown();
  void SetCursorCell(Vec2D c) { cursor_cell_ = c; }

 protected:
  Vec2D cursor_cell_;

 private:
  bool  scroll_left_;
  bool  scroll_right_;
  bool  scroll_up_;
  bool  scroll_down_;
};

class StateUIView : public StateUIOperable {
 public:
  StateUIView(StateUI::Base);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
  virtual void Update() override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIView"; }
#endif
};

// StateUIUnitSelected

class PathTree;

class StateUIUnitSelected : public StateUIOperable {
 public:
  StateUIUnitSelected(StateUI::Base, Unit*, PathTree*);
  ~StateUIUnitSelected();
  std::vector<Vec2D> GetPathToRoot(Vec2D pos);
  Unit* GetUnit() { return unit_; }
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;

#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIUnitSelected"; }
#endif

 private:
  Unit* unit_;
  PathTree* pathtree_;
  Vec2D origin_coords_;
};

// StateUIMoving

class Unit;

class StateUIMoving : public StateUI {
 public:
  enum class Flag {
    kNone,
    kInputActNext
  };

 public:
  static const int kFramesPerCell = 12;

 public:
  StateUIMoving(StateUI::Base, Unit*, const std::vector<Vec2D>&, Flag = Flag::kInputActNext);
  StateUIMoving(StateUI::Base, Unit*, Vec2D, Flag = Flag::kInputActNext);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIMoving"; }
#endif

 private:
  int CalcPathIdx();
  int NumPaths();
  bool LastFrame();

 private:
  Unit*         unit_;
  Vec2D         dest_;
  vector<Vec2D> path_;
  int           frames_;
  Flag          flag_;
};

// StateUIMagic

class Magic;
class TextureAnimator;

class StateUIMagic : public StateUI {
 public:
  StateUIMagic(StateUI::Base, Unit*, Unit*, Magic*, bool, int);
  virtual ~StateUIMagic();
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;

#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIMagic"; }
#endif

 public:
  static const int kFramesPerCut = 5;

 private:
  Unit*  atk_;
  Unit*  def_;
  Magic* magic_;
  bool   hit_;
  int    damage_;
  TextureAnimator* animator_;
};

// StateUIKilled

class StateUIKilled : public StateUI {
 public:
  StateUIKilled(StateUI::Base, Unit*);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIKilled"; }
#endif

 public:
  static const int kStateDuration = 80;

 private:
  Unit*    unit_;
  int      frames_;
};

// StateUIEmptySelected

class TerrainInfoView;

class StateUIEmptySelected : public StateUI {
 public:
  StateUIEmptySelected(StateUI::Base, Vec2D);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIEmptySelected"; }
#endif

 private:
  Vec2D coords_;
  TerrainInfoView* terrain_info_view_;
};

// StateUIAttack

class StateUIAttack : public StateUI {
 public:
  StateUIAttack(StateUI::Base, Unit*, Unit*, bool, bool, int);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIAttack"; }
#endif

 public:
  static const int kFramesPerCut  = 8;
  static const int kNumCuts       = 10;
  static const int kStateDuration = kFramesPerCut * kNumCuts;

 private:
  bool LastFrame() { return frames_ == kNumCuts * kFramesPerCut - 1; }

 private:
  Unit* atk_;
  Unit* def_;
  bool  hit_;
  bool  critical_;
  int   damage_;
  int   frames_;
};

// StateUIAction

class Unit;
class MagicListView;
class UnitInfoView;

class StateUIAction : public StateUIOperable {
 public:
  StateUIAction(StateUI::Base, Unit*, const string& = "basic_attack");
  ~StateUIAction();
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIAction"; }
#endif

 private:
  Unit* unit_;
  string magic_id_;
  Vec2D* range_itr_;
  bool is_basic_attack_;
};

class StateUINextTurn : public StateUI {
 public:
  StateUINextTurn(StateUI::Base);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Update() override;
  virtual void Render(Drawer*) override;
//  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
//  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUINextTurn"; }
#endif

 private:
  int frames_;
};

class StateUISpeak : public StateUI {
 public:
  StateUISpeak(StateUI::Base, Unit*, const string&);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;

 private:
  Unit* unit_;
  string words_;
};

class StateUIEnd : public StateUI {
 public:
  StateUIEnd(StateUI::Base);
  virtual void Enter() override;
};

#endif

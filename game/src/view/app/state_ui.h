#ifndef STATE_UI_H_
#define STATE_UI_H_

#include "util/state.h"
#include "view/foundation/i_view.h"

// StateUI

namespace mengde {
namespace core {
  class Game;
  class Magic;
  class PathTree;
  class Unit;
}
}

class RootView;
class Drawer;
class TextureAnimator;
class MagicListView;
class UnitInfoView;

class StateUI : public State, IView {
 public:
  struct Base {
    mengde::core::Game*     game;
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
  mengde::core::Game*     game_;
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

class StateUIUnitSelected : public StateUIOperable {
 public:
  StateUIUnitSelected(StateUI::Base, mengde::core::Unit*, mengde::core::PathTree*);
  ~StateUIUnitSelected();
  std::vector<Vec2D> GetPathToRoot(Vec2D pos);
  mengde::core::Unit* GetUnit() { return unit_; }
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;

#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIUnitSelected"; }
#endif

 private:
  mengde::core::Unit* unit_;
  mengde::core::PathTree* pathtree_;
  Vec2D origin_coords_;
};

// StateUIMoving

class StateUIMoving : public StateUI {
 public:
  enum class Flag {
    kNone,
    kInputActNext
  };

 public:
  static const int kFramesPerCell = 12;

 public:
  StateUIMoving(StateUI::Base, mengde::core::Unit*, const std::vector<Vec2D>&, Flag = Flag::kInputActNext);
  StateUIMoving(StateUI::Base, mengde::core::Unit*, Vec2D, Flag = Flag::kInputActNext);
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
  mengde::core::Unit*         unit_;
  Vec2D         dest_;
  vector<Vec2D> path_;
  int           frames_;
  Flag          flag_;
};

// StateUIMagic

class StateUIMagic : public StateUI {
 public:
  StateUIMagic(StateUI::Base, mengde::core::Unit*, mengde::core::Unit*, mengde::core::Magic*, bool, int);
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
  mengde::core::Unit*  atk_;
  mengde::core::Unit*  def_;
  mengde::core::Magic* magic_;
  bool   hit_;
  int    damage_;
  TextureAnimator* animator_;
};

// StateUIKilled

class StateUIKilled : public StateUI {
 public:
  StateUIKilled(StateUI::Base, mengde::core::Unit*);
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
  mengde::core::Unit*    unit_;
  int      frames_;
};

// StateUIEmptySelected

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
};

// StateUIAttack

class StateUIAttack : public StateUI {
 public:
  StateUIAttack(StateUI::Base, mengde::core::Unit*, mengde::core::Unit*, bool, bool, int);
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
  mengde::core::Unit* atk_;
  mengde::core::Unit* def_;
  bool  hit_;
  bool  critical_;
  int   damage_;
  int   frames_;
};

class StateUIDamaged : public StateUI {
 public:
  static const int kFrames = 90;

 public:
  StateUIDamaged(StateUI::Base, mengde::core::Unit*, int);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Update() override;
  virtual void Render(Drawer*) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIDamaged"; }
#endif

  bool LastFrame() { return frames_ == kFrames - 1; }

 private:
  int   frames_;
  mengde::core::Unit* unit_;
  int   damage_;
};

// StateUIAction

class StateUIAction : public StateUIOperable {
 public:
  StateUIAction(StateUI::Base, mengde::core::Unit*);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIAction"; }
#endif

 private:
  mengde::core::Unit* unit_;
};

// StateUIMagicSelection

class StateUIMagicSelection : public StateUIOperable {
 public:
  StateUIMagicSelection(StateUI::Base, mengde::core::Unit*);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIMagicSelection"; }
#endif

 private:
  mengde::core::Unit* unit_;
};

// StateUITargeting

class StateUITargeting : public StateUIOperable {
 public:
  StateUITargeting(StateUI::Base, mengde::core::Unit*, const string& = "basic_attack");
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUITargeting"; }
#endif

 private:
  mengde::core::Unit* unit_;
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
  StateUISpeak(StateUI::Base, mengde::core::Unit*, const string&);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;

 private:
  mengde::core::Unit* unit_;
  string words_;
};

class StateUIEnd : public StateUI {
 public:
  StateUIEnd(StateUI::Base);
  virtual void Enter() override;
};

#endif

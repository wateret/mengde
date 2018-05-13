#ifndef MENGDE_GUI_APP_STATE_UI_H_
#define MENGDE_GUI_APP_STATE_UI_H_

#include "core/user_interface.h"
#include "gui/foundation/texture_animator.h"
#include "gui/uifw/i_view.h"
#include "util/state.h"

#include "common.h"

namespace mengde {
namespace core {
class AttackRange;
class Game;
class UserInterface;
class Magic;
class PathTree;
class Unit;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

// StateUI

class GameView;
class MagicListView;
class UnitTooltipView;

class StateUI : public State, public IView {
 public:
  struct Base {
    core::Game* game;  // TODO We should eventually remove this, use core::UserInterface instead.
    core::UserInterface* gi;
    GameView* gv;
  };

 public:
  StateUI(Base);
  virtual ~StateUI() = default;
  virtual void Enter() override {}
  virtual void Exit() override {}
  virtual void Render(Drawer*) override {}
  virtual void Update() override {}
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override { return false; }
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override { return true; }
  virtual bool OnMouseWheelEvent(const foundation::MouseWheelEvent&) override { return false; }
  Base WrapBase() { return {game_, gi_, gv_}; }

#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUI"; }
#endif

 protected:
  core::Game* game_;
  core::UserInterface* gi_;
  GameView* gv_;
};

// StateUIMain

class StateUIMain : public StateUI {
 public:
  StateUIMain(Base);
  //  virtual void Enter() override;
  //  virtual void Exit() override;
  //  virtual void Render(Drawer*) override;
  //  virtual void Update() override;
  //  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  //  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;

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
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;

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
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;
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
  bool scroll_left_;
  bool scroll_right_;
  bool scroll_up_;
  bool scroll_down_;
};

class StateUIView : public StateUIOperable {
 public:
  StateUIView(StateUI::Base);
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;
  virtual void Update() override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIView"; }
#endif

 private:
  core::AvailableUnits units_;
};

// StateUIUnitSelected

class StateUIUnitSelected : public StateUIOperable {
 public:
  StateUIUnitSelected(StateUI::Base, uint32_t unit_id);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;

#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIUnitSelected"; }
#endif

 private:
  uint32_t unit_id_;
  core::AvailableMoves moves_;
  Vec2D origin_coords_;
};

// StateUIMoving

class StateUIMoving : public StateUI {
 public:
  enum class Flag { kNone, kInputActNext };

 public:
  static const int kFramesPerCell = 12;

 public:
  StateUIMoving(StateUI::Base, uint32_t unit_id, Vec2D, Flag = Flag::kInputActNext, uint32_t move_id = 0);
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
  uint32_t unit_id_;
  Vec2D dest_;
  int frames_;
  Flag flag_;
  vector<Vec2D> path_;
  uint32_t move_id_;
};

// StateUIMagic

class StateUIMagic : public StateUI {
 public:
  StateUIMagic(StateUI::Base, core::Unit*, core::Unit*, core::Magic*, bool, int);
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
  core::Unit* atk_;
  core::Unit* def_;
  core::Magic* magic_;
  bool hit_;
  int damage_;
  TextureAnimator* animator_;
};

// StateUIKilled

class StateUIKilled : public StateUI {
 public:
  StateUIKilled(StateUI::Base, core::Unit*);
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
  core::Unit* unit_;
  int frames_;
};

// StateUIEmptySelected

class StateUIEmptySelected : public StateUI {
 public:
  StateUIEmptySelected(StateUI::Base, Vec2D);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIEmptySelected"; }
#endif

 private:
  Vec2D coords_;
};

// StateUIAttack

class StateUIAttack : public StateUI {
 public:
  StateUIAttack(StateUI::Base, core::Unit*, core::Unit*, bool, bool, int);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIAttack"; }
#endif

 public:
  static const int kFramesPerCut = 8;
  static const int kNumCuts = 10;
  static const int kStateDuration = kFramesPerCut * kNumCuts;

 private:
  bool LastFrame() { return frames_ == kNumCuts * kFramesPerCut - 1; }

 private:
  core::Unit* atk_;
  core::Unit* def_;
  bool hit_;
  bool critical_;
  int damage_;
  int frames_;
};

class StateUIUnitTooltipAnim : public StateUI {
 public:
  static const int kFrames = 90;

 public:
  StateUIUnitTooltipAnim(StateUI::Base, core::Unit*, int hp, int mp);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Update() override;
  virtual void Render(Drawer*) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIUnitTooltipAnim"; }
#endif

  bool LastFrame() { return frames_ == kFrames - 1; }

 private:
  int frames_;
  core::Unit* unit_;
  int hp_;  // Amount of hp changed
  int mp_;  // Amount of mp changed
};

// StateUIAction

class StateUIAction : public StateUI {
 public:
  StateUIAction(StateUI::Base, uint32_t unit_id, uint32_t move_id);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIAction"; }
#endif

 private:
  uint32_t unit_id_;
  uint32_t move_id_;
  Vec2D pos_;
};

// StateUIMagicSelection

class StateUIMagicSelection : public StateUI {
 public:
  StateUIMagicSelection(StateUI::Base, uint32_t unit_id, uint32_t move_id);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIMagicSelection"; }
#endif

 private:
  uint32_t unit_id_;
  uint32_t move_id_;
  Vec2D pos_;
};

// StateUITargeting

class StateUITargeting : public StateUIOperable {
 public:
  StateUITargeting(StateUI::Base, uint32_t unit_id, uint32_t move_id, const string& = "basic_attack");
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Render(Drawer*) override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUITargeting"; }
#endif

 private:
  const core::AttackRange& GetRange();

 private:
  uint32_t unit_id_;
  uint32_t move_id_;
  string magic_id_;
  bool is_basic_attack_;
  const core::AttackRange& range_;
  core::AvailableActs acts_;
};

class StateUINextTurn : public StateUI {
 public:
  StateUINextTurn(StateUI::Base);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Update() override;
  virtual void Render(Drawer*) override;
//  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
//  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent&) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUINextTurn"; }
#endif

 private:
  int frames_;
};

class StateUISpeak : public StateUI {
 public:
  StateUISpeak(StateUI::Base, core::Unit*, const string&);
  virtual void Enter() override;
  virtual void Exit() override;
  virtual void Update() override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUISpeak"; }
#endif

 private:
  core::Unit* unit_;
  string words_;
};

class StateUIEnd : public StateUI {
 public:
  StateUIEnd(StateUI::Base, bool is_victory);
  virtual void Enter() override;
  virtual void Update() override;
#ifdef DEBUG
  virtual string GetStateID() const override { return "StateUIEnd"; }
#endif

 private:
  bool is_victory_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_STATE_UI_H_

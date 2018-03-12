#include "state_ui.h"

#include "config.h"
#include "control_view.h"
#include "core/cmd.h"
#include "core/formulae.h"
#include "core/game.h"
#include "core/magic.h"
#include "core/magic_list.h"
#include "core/map.h"
#include "core/path_tree.h"
#include "core/unit.h"
#include "game_view.h"
#include "gui/foundation/misc.h"
#include "gui/foundation/texture_animator.h"
#include "gui/foundation/texture_manager.h"
#include "gui/uifw/drawer.h"
#include "gui/uifw/modal_dialog_view.h"
#include "gui/uifw/modal_view.h"
#include "layout_helper.h"
#include "magic_list_view.h"
#include "resource_path.h"
#include "terrain_info_view.h"
#include "unit_action_view.h"
#include "unit_dialog_view.h"
#include "unit_list_view.h"
#include "unit_tooltip_view.h"
#include "unit_view.h"

namespace mengde {
namespace gui {
namespace app {

// StateUI

StateUI::StateUI(Base base) : game_(base.game), gv_(base.gv) {}

// StateUIMain

StateUIMain::StateUIMain(StateUI::Base base) : StateUI(base) {}

// StateUIDoCmd

StateUIDoCmd::StateUIDoCmd(StateUI::Base base) : StateUI(base), cmd_to_do_(false) {}

void StateUIDoCmd::Enter() {
  // FIXME Avoid flag based operations
  //       Can we do the command before StateUIView?
  if (cmd_to_do_) {
    // Run reserved command run
    ASSERT(game_->HasNext());
    game_->DoNext();
    cmd_to_do_ = false;
  }
  while (game_->HasNext()) {
    StateUI* state = GenerateNextCmdUIState();
    if (state != nullptr) {
      gv_->PushUIState(state);
      // Reserve doing the command. Cmd will be run when coming back(pop from the pushed state) to this State
      cmd_to_do_ = true;
      break;
    } else {
      // If we do not have UIState for the command do it immediately
      game_->DoNext();
    }
  }
}

// Generate corresponding StateUI for next Cmd
// Returns nullptr when no UI needed
StateUI* StateUIDoCmd::GenerateNextCmdUIState() {
  ASSERT(game_->HasNext());
  const core::Cmd* cmd         = game_->GetNextCmdConst();
  StateUI*         no_state_ui = nullptr;

#define DYNAMIC_CAST_CHECK(type)  \
  dynamic_cast<const type*>(cmd); \
  ASSERT(c != nullptr);

  switch (cmd->GetOp()) {
    case core::Cmd::Op::kCmdAction:
    case core::Cmd::Op::kCmdPlayAI:
    case core::Cmd::Op::kCmdGameVictory:
      return no_state_ui;

    case core::Cmd::Op::kCmdHit: {
      const core::CmdHit* c = DYNAMIC_CAST_CHECK(core::CmdHit);
      if (c->IsBasicAttack()) {
        return new StateUIAttack(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), true,
                                 c->GetHitType() == core::CmdHit::HitType::kCritical, c->GetDamage());
      } else {
        ASSERT(c->IsMagic());
        return new StateUIMagic(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), c->GetMagic(), true, c->GetDamage());
      }
    }
    case core::Cmd::Op::kCmdMiss: {
      const core::CmdMiss* c = DYNAMIC_CAST_CHECK(core::CmdMiss);
      if (c->IsBasicAttack()) {
        return new StateUIAttack(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), false, false, 0);
      } else {
        ASSERT(c->IsMagic());
        return new StateUIMagic(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), c->GetMagic(), false, 0);
      }
    }
    case core::Cmd::Op::kCmdMove: {
      const core::CmdMove* c    = DYNAMIC_CAST_CHECK(core::CmdMove);
      StateUIMoving::Flag  flag = StateUIMoving::Flag::kNone;
      if (c->GetUnit()->GetPosition() == c->GetDest()) {
        return no_state_ui;
      } else {
        return new StateUIMoving(WrapBase(), c->GetUnit(), c->GetDest(), flag);
      }
    }
    case core::Cmd::Op::kCmdKilled: {
      const core::CmdKilled* c = DYNAMIC_CAST_CHECK(core::CmdKilled);
      return new StateUIKilled(WrapBase(), c->GetUnit());
    }
    case core::Cmd::Op::kCmdEndTurn:
      return new StateUINextTurn(WrapBase());

    case core::Cmd::Op::kCmdSpeak: {
      const core::CmdSpeak* c = DYNAMIC_CAST_CHECK(core::CmdSpeak);
      return new StateUISpeak(WrapBase(), c->GetUnit(), c->GetWords());
    }
    case core::Cmd::Op::kCmdGameEnd: {
      const core::CmdGameEnd* c = DYNAMIC_CAST_CHECK(core::CmdGameEnd);
      return new StateUIEnd(WrapBase(), c->is_victory());
    }
    case core::Cmd::Op::kCmdRestoreHp: {
      const core::CmdRestoreHp* c      = DYNAMIC_CAST_CHECK(core::CmdRestoreHp);
      int                       amount = c->CalcAmount();
      if (amount == 0) return no_state_ui;
      return new StateUIUnitTooltipAnim(WrapBase(), c->GetUnit(), c->CalcAmount(), 0);
    }

    default:
      //      UNREACHABLE("Unknown type of Cmd");
      return no_state_ui;
  }

  return no_state_ui;
#undef DYNAMIC_CAST_CHECK
}

void StateUIDoCmd::Exit() {}

void StateUIDoCmd::Render(Drawer*) {}

void StateUIDoCmd::Update() {
  if (!game_->HasNext()) {
    gv_->PopUIState();
  }
}

bool StateUIDoCmd::OnMouseButtonEvent(const foundation::MouseButtonEvent) { return false; }

bool StateUIDoCmd::OnMouseMotionEvent(const foundation::MouseMotionEvent) { return true; }

// StateUIOperable

StateUIOperable::StateUIOperable(Base base) : StateUI(base), cursor_cell_(0, 0) { ClearScrolls(); }

void StateUIOperable::Enter() {}

void StateUIOperable::Exit() {
  gv_->unit_tooltip_view()->visible(false);
  gv_->unit_view()->visible(false);
}

void StateUIOperable::Update() {
  // Update camera coords
  static const int kScrollAmount = 8;
  if (IsScrollLeft()) gv_->MoveCameraX(-kScrollAmount);
  if (IsScrollRight()) gv_->MoveCameraX(kScrollAmount);
  if (IsScrollUp()) gv_->MoveCameraY(-kScrollAmount);
  if (IsScrollDown()) gv_->MoveCameraY(kScrollAmount);
  cursor_cell_ = (gv_->GetMouseCoords() + gv_->GetCameraCoords()) / config::kBlockSize;
}

void StateUIOperable::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(255, 255, 255, 128));
  drawer->BorderCell(cursor_cell_, 4);
}

bool StateUIOperable::OnMouseMotionEvent(const foundation::MouseMotionEvent e) {
  if (e.IsMotionOver()) {
    // mouse scroll
    const int window_width  = gv_->GetFrameSize().x;
    const int window_height = gv_->GetFrameSize().y;
    const int kScrollRange  = config::kBlockSize;
    const int kLeftScroll   = kScrollRange;
    const int kRightScroll  = window_width - kScrollRange;
    const int kUpScroll     = kScrollRange;
    const int kDownScroll   = window_height - kScrollRange;

    ClearScrolls();
    Vec2D mouse_coords = gv_->GetMouseCoords();
    if (mouse_coords.x < kLeftScroll) SetScrollLeft();
    if (mouse_coords.x > kRightScroll) SetScrollRight();
    if (mouse_coords.y < kUpScroll) SetScrollUp();
    if (mouse_coords.y > kDownScroll) SetScrollDown();
  }
  return false;
}

void StateUIOperable::ClearScrolls() { scroll_left_ = scroll_right_ = scroll_up_ = scroll_down_ = false; }

void StateUIOperable::SetScrollLeft() { scroll_left_ = true; }

void StateUIOperable::SetScrollRight() { scroll_right_ = true; }

void StateUIOperable::SetScrollUp() { scroll_up_ = true; }

void StateUIOperable::SetScrollDown() { scroll_down_ = true; }

bool StateUIOperable::IsScrollLeft() { return scroll_left_; }

bool StateUIOperable::IsScrollRight() { return scroll_right_; }

bool StateUIOperable::IsScrollUp() { return scroll_up_; }

bool StateUIOperable::IsScrollDown() { return scroll_down_; }

// StateUIView

StateUIView::StateUIView(Base base) : StateUIOperable(base) {}

void StateUIView::Update() {
  StateUIOperable::Update();

  core::Map* map = game_->GetMap();
  if (map->UnitInCell(cursor_cell_)) {
    core::Cell* cell = map->GetCell(cursor_cell_);
    core::Unit* unit = map->GetUnit(cursor_cell_);
    gv_->unit_tooltip_view()->SetUnitTerrainInfo(cell);
    gv_->unit_tooltip_view()->SetCoordsByUnitCoords(unit->GetPosition(), gv_->GetCameraCoords(), gv_->GetFrameSize());
    gv_->unit_view()->SetUnit(unit);

    gv_->unit_tooltip_view()->visible(true);
  } else {
    gv_->unit_tooltip_view()->visible(false);
    gv_->unit_view()->visible(false);
  }

  if (game_->IsAITurn()) {
    game_->Push(unique_ptr<core::CmdPlayAI>(new core::CmdPlayAI()));
  }

  if (game_->HasNext()) {
    gv_->PushUIState(new StateUIDoCmd({game_, gv_}));
  }
}

bool StateUIView::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    Vec2D      pos = GetCursorCell();
    core::Map* map = game_->GetMap();
    if (map->UnitInCell(pos)) {
      core::Unit* unit = map->GetUnit(pos);
      if (game_->IsCurrentTurn(unit)) {
        if (unit->IsDoneAction()) {
          LOG_INFO("The chosen unit is already done his action");
        } else {
          core::PathTree* pathtree = map->FindMovablePath(unit);
          gv_->PushUIState(new StateUIUnitSelected(WrapBase(), unit, pathtree));
        }
      } else {
        // TODO alert this is not your unit
      }
    } else {
      gv_->PushUIState(new StateUIEmptySelected(WrapBase(), pos));
    }
  } else if (e.IsRightButtonUp()) {
    Vec2D      pos = GetCursorCell();
    core::Map* map = game_->GetMap();
    if (map->UnitInCell(pos)) {
      core::Unit* unit = map->GetUnit(pos);
      gv_->unit_list_view()->SetUnit(unit);
      gv_->unit_list_view()->visible(true);
    }
  }
  return true;
}

bool StateUIView::OnMouseMotionEvent(const foundation::MouseMotionEvent e) {
  return StateUIOperable::OnMouseMotionEvent(e);
}

// StateUIUnitSelected

StateUIUnitSelected::StateUIUnitSelected(StateUI::Base base, core::Unit* unit, core::PathTree* pathtree)
    : StateUIOperable(base), unit_(unit), pathtree_(pathtree), origin_coords_(unit_->GetPosition()) {}

StateUIUnitSelected::~StateUIUnitSelected() {
  if (pathtree_ != nullptr) delete pathtree_;
}

void StateUIUnitSelected::Enter() { game_->MoveUnit(unit_, origin_coords_); }

void StateUIUnitSelected::Exit() {}

void StateUIUnitSelected::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(unit_->GetPosition(), 4);

  std::vector<Vec2D> movable_cells = pathtree_->GetNodeList();
  drawer->SetDrawColor(Color(0, 0, 192, 96));
  core::Map* map = game_->GetMap();
  for (auto cell : movable_cells) {
    if (!map->UnitInCell(cell)) {
      drawer->FillCell(cell);
    }
  }

  StateUIOperable::Render(drawer);
}

void StateUIUnitSelected::Update() {
  StateUIOperable::Update();

  auto unit_view = gv_->unit_view();
  unit_view->SetUnit(unit_);
  unit_view->visible(true);
}

bool StateUIUnitSelected::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    std::vector<Vec2D> movable_cells = pathtree_->GetNodeList();
    Vec2D              pos           = GetCursorCell();
    core::Map*         map           = game_->GetMap();
    if (map->UnitInCell(pos) && map->GetUnit(pos) != unit_) {
      // XXX Other unit clicked
    } else if (std::find(movable_cells.begin(), movable_cells.end(), pos) != movable_cells.end()) {
      gv_->PushUIState(new StateUIMoving({game_, gv_}, unit_, GetPathToRoot(pos)));
    } else {
      //      gv_->ChangeStateUI(new StateUIView(game_, gv_));
    }
  } else if (e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

std::vector<Vec2D> StateUIUnitSelected::GetPathToRoot(Vec2D pos) { return pathtree_->GetPathToRoot(pos); }

// StateUIMoving

StateUIMoving::StateUIMoving(StateUI::Base base, core::Unit* unit, Vec2D dest, Flag flag)
    : StateUI(base), unit_(unit), dest_(dest), path_(), frames_(-1), flag_(flag) {}

StateUIMoving::StateUIMoving(StateUI::Base base, core::Unit* unit, const vector<Vec2D>& path, Flag flag)
    : StateUI(base), unit_(unit), dest_(), path_(path), frames_(-1), flag_(flag) {
  ASSERT(!path_.empty());
  dest_ = path_[0];
}

int StateUIMoving::NumPaths() { return static_cast<int>(path_.size()) - 1; }

int StateUIMoving::CalcPathIdx() { return (int)(path_.size() - 1) - ((frames_) / kFramesPerCell); }

bool StateUIMoving::LastFrame() {
  if (NumPaths() == 0) {
    return true;
  }
  int last_frame = (NumPaths() * kFramesPerCell) - 1;
  ASSERT_LE(frames_, last_frame);
  return frames_ == last_frame;
}

void StateUIMoving::Enter() {
  unit_->SetNoRender(true);

  if (path_.empty()) {
    // Lazy path generation
    core::Map* m = game_->GetMap();
    path_        = m->FindPathTo(unit_, dest_);
    ASSERT(dest_ == path_[0]);
  }
}

void StateUIMoving::Exit() { unit_->SetNoRender(false); }

void StateUIMoving::Update() {
  frames_++;
  if (LastFrame()) {  // Arrived at the destination
    ASSERT(dest_ == path_[0]);
    game_->MoveUnit(unit_, dest_);
    if (path_.size() > 1) {
      Direction dir = Vec2DRelativePosition(path_[1], path_[0]);
      unit_->SetDirection(dir);  // Do this here?
    }
    if (flag_ == Flag::kInputActNext) {
      gv_->ChangeUIState(new StateUIAction(WrapBase(), unit_));
    } else {
      gv_->PopUIState();
    }
  }
}

void StateUIMoving::Render(Drawer* drawer) {
  if (NumPaths() == 0) {
    return;
  }
  ASSERT_LT(frames_, NumPaths() * kFramesPerCell);

  int path_idx = CalcPathIdx();
  ASSERT_GT(path_idx, 0);

  int       frames_current = frames_ % kFramesPerCell;
  int       sprite_no      = frames_current / (kFramesPerCell / 2);
  float     percentage     = frames_current / (float)kFramesPerCell;
  Direction dir            = Vec2DRelativePosition(path_[path_idx], path_[path_idx - 1]);
  Vec2D     diff           = path_[path_idx - 1] - path_[path_idx];
  Vec2D     diff_pos       = diff * (percentage * (float)config::kBlockSize);
  drawer->CopySprite(unit_->GetModelId(), kSpriteMove, dir, sprite_no, {kEffectNone, 0}, path_[path_idx], diff_pos);
  gv_->CenterCamera(path_[path_idx] * config::kBlockSize + diff_pos + (config::kBlockSize / 2));
}

// StateUIMagic

StateUIMagic::StateUIMagic(StateUI::Base base, core::Unit* atk, core::Unit* def, core::Magic* magic, bool hit,
                           int damage)
    : StateUI(base), atk_(atk), def_(def), magic_(magic), hit_(hit), damage_(damage), animator_(nullptr) {}

StateUIMagic::~StateUIMagic() {
  ASSERT(animator_ != NULL);
  delete animator_;
}

void StateUIMagic::Enter() {
  atk_->SetNoRender(true);
  def_->SetNoRender(true);
}

void StateUIMagic::Exit() {
  atk_->SetNoRender(false);
  def_->SetNoRender(false);
}

void StateUIMagic::Render(Drawer* drawer) {
  if (animator_ == NULL) {
    TextureManager* tm      = drawer->GetTextureManager();
    Texture*        texture = tm->FetchTexture(rcpath::MagicPath(magic_->GetId()).ToString());
    texture->SetAlpha(160);  // FIXME non-fixed alpha value
    animator_ = new TextureAnimator(texture, kFramesPerCut);
  }

  Vec2D     unit_pos = atk_->GetPosition();
  Vec2D     def_pos  = def_->GetPosition();
  Direction dir      = Vec2DRelativePosition(unit_pos, def_pos);

  drawer->CopySprite(atk_->GetModelId(), kSpriteAttack, dir, 0, {kEffectNone, 0}, unit_pos);

  const SpriteType target_sprite_hit =
      magic_->IsTypeDeal() ? kSpriteDamaged : kSpriteBuff;  // This condition may not be accurate
  const SpriteType target_sprite = hit_ ? target_sprite_hit : kSpriteBlocked;

  drawer->CopySprite(def_->GetModelId(), target_sprite, OppositeDirection(dir), 0, {kEffectNone, 0}, def_pos);

  Rect src_rect = animator_->GetCurrentCutRect();
  drawer->CopyTextureToCell(animator_->GetTexture(), &src_rect, def_pos);
}

void StateUIMagic::Update() {
  if (animator_) {
    animator_->NextFrame();
    if (animator_->DoneAnimate()) {
      if (hit_) {
        gv_->ChangeUIState(new StateUIUnitTooltipAnim(WrapBase(), def_, -damage_, 0));
      } else {
        gv_->PopUIState();
      }
    }
  }
}

// StateUIKilled

StateUIKilled::StateUIKilled(StateUI::Base base, core::Unit* unit) : StateUI(base), unit_(unit), frames_(0) {}

void StateUIKilled::Enter() {
  unit_->SetNoRender(true);
  Misc::SetShowCursor(false);
}

void StateUIKilled::Exit() {
  unit_->SetNoRender(false);
  Misc::SetShowCursor(true);
}

void StateUIKilled::Render(Drawer* drawer) {
  const int wait     = 20;
  const int hold     = 20;
  int       progress = 0;
  if (frames_ >= wait) {
    int f = frames_ - wait;
    if (f > hold) progress = (f - hold) * 255 / (kStateDuration - hold);
  }
  drawer->CopySprite(unit_->GetModelId(), kSpriteLowHP, kDirNone, 0, {kEffectShade, progress}, unit_->GetPosition());
}

void StateUIKilled::Update() {
  frames_++;
  if (frames_ == kStateDuration) {
    gv_->PopUIState();
  }
}

// StateUIEmptySelected

StateUIEmptySelected::StateUIEmptySelected(Base base, Vec2D coords) : StateUI(base), coords_(coords) {}

void StateUIEmptySelected::Enter() {
  core::Map*  map  = game_->GetMap();
  std::string name = map->GetTerrain(coords_)->GetName();
  gv_->terrain_info_view()->SetText(name);
  gv_->terrain_info_view()->visible(true);
}

void StateUIEmptySelected::Exit() { gv_->terrain_info_view()->visible(false); }

void StateUIEmptySelected::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(coords_, 4);
}

bool StateUIEmptySelected::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (e.IsLeftButtonUp() || e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

// StateUIAttack

StateUIAttack::StateUIAttack(StateUI::Base base, core::Unit* atk, core::Unit* def, bool hit, bool critical, int damage)
    : StateUI(base), atk_(atk), def_(def), hit_(hit), critical_(critical), damage_(damage), frames_(-1) {}

void StateUIAttack::Enter() {
  atk_->SetNoRender(true);
  def_->SetNoRender(true);
  Misc::SetShowCursor(false);
}

void StateUIAttack::Exit() {
  atk_->SetNoRender(false);
  def_->SetNoRender(false);
  Misc::SetShowCursor(true);
}

void StateUIAttack::Render(Drawer* drawer) {
  // Render units

  // BasicAttack Animation Order
  //
  // [NO]   [ATTACKER]    [DEFENDER(HIT)]    [DEFENDER(MISS)]
  //   0    Stand[0]      Stand[0]           Stand[0]
  //   1    Stand[0]      Stand[0]           Stand[0]
  //   2    Attack[0]     Stand[0]           Stand[0]
  //   3    Attack[0]     Stand[0]           Stand[0]
  //   4    Attack[0]     Stand[0]           Stand[0]
  //   5    Attack[1]     Stand[0]           Stand[0]
  //   6    Attack[2]     Damaged[0]         Blocked[0]
  //   7    Attack[3]     Damaged[0]         Blocked[0]
  //   8    Attack[3]     Damaged[0]         Blocked[0]
  //   9    Attack[3]     Damaged[0]         Blocked[0]

  struct CutInfo {
    SpriteType sprite;
    int        no;
    int        offset;
  };

  static const CutInfo kCutInfoAtk[kNumCuts] = {{kSpriteStand, 0, 0},  {kSpriteStand, 0, 0},  {kSpriteAttack, 0, 0},
                                                {kSpriteAttack, 0, 0}, {kSpriteAttack, 0, 0}, {kSpriteAttack, 1, 0},
                                                {kSpriteAttack, 2, 8}, {kSpriteAttack, 3, 8}, {kSpriteAttack, 3, 8},
                                                {kSpriteAttack, 3, 8}};

  static const CutInfo kCutInfoDefDamaged[kNumCuts] = {
      {kSpriteStand, 0, 0},   {kSpriteStand, 0, 0},  {kSpriteStand, 0, 0},   {kSpriteStand, 0, 0},
      {kSpriteStand, 0, 0},   {kSpriteStand, 0, 0},  {kSpriteDamaged, 0, 0}, {kSpriteDamaged, 0, 0},
      {kSpriteDamaged, 0, 0}, {kSpriteDamaged, 0, 0}};

  static const CutInfo kCutInfoDefBlocked[kNumCuts] = {
      {kSpriteStand, 0, 0},    {kSpriteStand, 0, 0},   {kSpriteStand, 0, 0},    {kSpriteStand, 0, 0},
      {kSpriteStand, 0, 0},    {kSpriteStand, 0, 0},   {kSpriteBlocked, 0, -4}, {kSpriteBlocked, 0, -4},
      {kSpriteBlocked, 0, -4}, {kSpriteBlocked, 0, -4}};

  int cut_no = frames_ / kFramesPerCut;
  ASSERT(cut_no < kNumCuts);
  const CutInfo* cut_atk    = &kCutInfoAtk[cut_no];
  const CutInfo* cut_def    = hit_ ? &kCutInfoDefDamaged[cut_no] : &kCutInfoDefBlocked[cut_no];
  Vec2D          atk_pos    = atk_->GetPosition();
  Vec2D          def_pos    = def_->GetPosition();
  Vec2D          atk_offset = GenerateVec2DOffset(atk_->GetDirection(), cut_atk->offset);
  Vec2D          def_offset = GenerateVec2DOffset(def_->GetDirection(), cut_def->offset);
  Direction      dir        = Vec2DRelativePosition(atk_pos, def_pos);

  SpriteEffect sprite_effect = {kEffectNone, 0};
  if (critical_) {
    const int maxp        = 224;
    const int unit_frames = kStateDuration / 5;
    int       progress    = 0;
    if (frames_ < unit_frames) {
      progress = frames_ * maxp / unit_frames;
    } else if (frames_ < unit_frames * 2) {
      progress = maxp;
    } else if (frames_ < unit_frames * 3) {
      progress = maxp - (frames_ - unit_frames * 2) * maxp / unit_frames;
    }
    sprite_effect = {kEffectBright, progress};
  }

  if (atk_->GetPosition().y > def_->GetPosition().y) {
    drawer->CopySprite(def_->GetModelId(), cut_def->sprite, OppositeDirection(dir), cut_def->no, {kEffectNone, 0},
                       def_pos, def_offset);
    drawer->CopySprite(atk_->GetModelId(), cut_atk->sprite, dir, cut_atk->no, sprite_effect, atk_pos, atk_offset);
  } else {
    drawer->CopySprite(atk_->GetModelId(), cut_atk->sprite, dir, cut_atk->no, sprite_effect, atk_pos, atk_offset);
    drawer->CopySprite(def_->GetModelId(), cut_def->sprite, OppositeDirection(dir), cut_def->no, {kEffectNone, 0},
                       def_pos, def_offset);
  }

  // Render Damage Text
  if (frames_ >= 5 * kFramesPerCut) {
    const int text_size       = 13;
    Vec2D     damage_text_pos = def_pos * config::kBlockSize;
    string    damage_text;
    if (!hit_) {
      damage_text = "miss";
    } else {
      damage_text = std::to_string(damage_);
    }

    // FIXME inefficient border effect
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        Vec2D border_pos = {i, j};
        drawer->DrawText(damage_text, text_size, COLOR("white"), damage_text_pos + border_pos);
      }
    }
    drawer->DrawText(damage_text, text_size, {228, 32, 32, 255}, damage_text_pos);
  }
}

void StateUIAttack::Update() {
  frames_++;
  if (LastFrame()) {
    if (hit_) {
      gv_->ChangeUIState(new StateUIUnitTooltipAnim(WrapBase(), def_, -damage_, 0));
    } else {
      gv_->PopUIState();
    }
  }
}

// StateUIUnitTooltipAnim

StateUIUnitTooltipAnim::StateUIUnitTooltipAnim(StateUI::Base base, core::Unit* unit, int hp, int mp)
    : StateUI(base), frames_(-1), unit_(unit), hp_(hp), mp_(mp) {
  const core::HpMp& cur_hpmp = unit->GetCurrentHpMp();
  const core::HpMp& max_hpmp = unit->GetOriginalHpMp();
  int               cur_hp   = cur_hpmp.hp;
  int               cur_mp   = cur_hpmp.mp;
  int               max_hp   = max_hpmp.hp;
  int               max_mp   = max_hpmp.mp;

  // Cap min/max value of changes
  if (hp_ > 0) {
    hp_ = std::min(hp_, max_hp - cur_hp);
  } else {
    hp_ = std::max(hp_, -cur_hp);
  }
  if (mp_ > 0) {
    mp_ = std::min(mp_, max_mp - cur_mp);
  } else {
    mp_ = std::max(mp_, -cur_mp);
  }
}

void StateUIUnitTooltipAnim::Enter() {
  Misc::SetShowCursor(false);
  gv_->unit_tooltip_view()->visible(true);
}

void StateUIUnitTooltipAnim::Exit() {
  Misc::SetShowCursor(true);
  gv_->unit_tooltip_view()->visible(false);
}

void StateUIUnitTooltipAnim::Update() {
  frames_++;
  if (LastFrame()) {
    gv_->PopUIState();
  }

  const int max_anim_frames = (kFrames - 1) * 1 / 2;
  const int cur_anim_frames = std::min(max_anim_frames, frames_);

  core::HpMp hpmp_mod = unit_->GetCurrentHpMp();
  core::HpMp hpmp_rem = {0, 0};
  if (hp_ < 0) {
    hpmp_rem.hp = -hp_ * (max_anim_frames - cur_anim_frames) / max_anim_frames;
    hpmp_mod.hp -= -hp_ - hpmp_rem.hp;
  } else {
    hpmp_rem.hp = 0;
    hpmp_mod.hp += hp_ * cur_anim_frames / max_anim_frames;
  }
  if (mp_ < 0) {
    hpmp_rem.mp = -mp_ * (max_anim_frames - cur_anim_frames) / max_anim_frames;
    hpmp_mod.mp -= -mp_ - hpmp_rem.mp;
  } else {
    hpmp_rem.mp = 0;
    hpmp_mod.mp += mp_ * cur_anim_frames / max_anim_frames;
  }

  gv_->unit_tooltip_view()->SetContents(unit_->GetId(), unit_->GetLevel(), hpmp_mod, unit_->GetOriginalHpMp(),
                                        hpmp_rem);
  gv_->unit_tooltip_view()->SetCoordsByUnitCoords(unit_->GetPosition(), gv_->GetCameraCoords(), gv_->GetFrameSize());
}

void StateUIUnitTooltipAnim::Render(Drawer*) {}

// StateUITargeting

StateUITargeting::StateUITargeting(StateUI::Base base, core::Unit* unit, const string& magic_id)
    : StateUIOperable(base), unit_(unit), magic_id_(magic_id), range_itr_(NULL), is_basic_attack_(true) {
  if (!magic_id.compare("basic_attack")) {
    is_basic_attack_ = true;
    range_itr_       = unit_->GetAttackRange();
  } else {
    is_basic_attack_   = false;
    core::Magic* magic = game_->GetMagic(magic_id_);
    range_itr_         = magic->GetRange();  // FIXME
  }
  Rect frame = LayoutHelper::CalcPosition(gv_->GetFrameSize(), {200, 100}, LayoutHelper::kAlignLftBot,
                                          LayoutHelper::kDefaultSpace);
  frame.Move(frame.GetW() + LayoutHelper::kDefaultSpace, 0);
}

void StateUITargeting::Enter() { StateUIOperable::Enter(); }

void StateUITargeting::Exit() {
  gv_->unit_tooltip_view()->visible(false);
  StateUIOperable::Exit();
}

void StateUITargeting::Render(Drawer* drawer) {
  // Show Attack Range
  ASSERT(range_itr_ != NULL);
  Vec2D*      range_itr = range_itr_;
  Vec2D       pos       = unit_->GetPosition();
  const Vec2D nil       = {0, 0};
  for (; *range_itr != nil; range_itr++) {
    Vec2D cpos = pos + *range_itr;
    if (!game_->IsValidCoords(cpos)) continue;
    drawer->SetDrawColor(Color(255, 64, 64, 128));
    drawer->FillCell(cpos);
  }

  StateUIOperable::Render(drawer);
}

void StateUITargeting::Update() { StateUIOperable::Update(); }

bool StateUITargeting::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    Vec2D      map_pos = GetCursorCell();
    core::Map* map     = game_->GetMap();
    if (map->UnitInCell(map_pos)) {
      core::Unit* atk = unit_;
      core::Unit* def = map->GetUnit(map_pos);
      if (is_basic_attack_) {
        if (atk->IsInRange(map_pos)) {
          if (atk->IsHostile(def)) {
            unique_ptr<core::CmdAction> action(new core::CmdAction());
            action->SetCmdMove(unique_ptr<core::CmdMove>(new core::CmdMove(atk, atk->GetPosition())));
            action->SetCmdAct(unique_ptr<core::CmdBasicAttack>(
                new core::CmdBasicAttack(atk, def, core::CmdBasicAttack::Type::kActive)));
            game_->Push(std::move(action));
            gv_->InitUIStateMachine();
          } else {
            LOG_INFO("Cannot attack the unit. It is not enemy");
          }
        }
      } else {
        core::Magic* magic = game_->GetMagic(magic_id_);
        if (atk->IsInRange(map_pos, magic->GetRange())) {
          if (atk->IsHostile(def) == magic->GetIsTargetEnemy()) {
            unique_ptr<core::CmdAction> action(new core::CmdAction());
            action->SetCmdMove(unique_ptr<core::CmdMove>(new core::CmdMove(atk, atk->GetPosition())));
            action->SetCmdAct(unique_ptr<core::CmdMagic>(new core::CmdMagic(atk, def, magic)));
            game_->Push(std::move(action));
            gv_->InitUIStateMachine();
          } else {
            // TODO alert
            LOG_INFO("Cannot perform magic to the unit chosen.");
          }
        }
      }
    }
  } else if (e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

bool StateUITargeting::OnMouseMotionEvent(const foundation::MouseMotionEvent e) {
  StateUIOperable::OnMouseMotionEvent(e);

  auto       unit_tooltip_view = gv_->unit_tooltip_view();
  core::Map* map               = game_->GetMap();
  Vec2D      cursor_cell       = GetCursorCell();
  bool       unit_in_cell      = map->UnitInCell(cursor_cell);
  if (unit_in_cell) {
    core::Unit* unit_target = map->GetUnit(cursor_cell);
    bool        hostile     = unit_->IsHostile(unit_target);
    if (hostile) {
      int damage = is_basic_attack_ ? core::Formulae::ComputeBasicAttackDamage(map, unit_, unit_target)
                                    : core::Formulae::ComputeMagicDamage(map, unit_, unit_target);
      int accuracy = is_basic_attack_ ? core::Formulae::ComputeBasicAttackAccuracy(unit_, unit_target)
                                      : core::Formulae::ComputeMagicAccuracy(unit_, unit_target);

      unit_tooltip_view->SetUnitAttackInfo(unit_target, accuracy, damage);
    } else {
      unit_tooltip_view->SetUnitTerrainInfo(map->GetCell(cursor_cell));
    }
    unit_tooltip_view->SetCoordsByUnitCoords(unit_target->GetPosition(), gv_->GetCameraCoords(), gv_->GetFrameSize());
  }
  unit_tooltip_view->visible(unit_in_cell);
  return true;
}

// StateUIAction

StateUIAction::StateUIAction(StateUI::Base base, core::Unit* unit) : StateUI(base), unit_(unit) {}

void StateUIAction::Enter() {
  UnitActionView* unit_action_view = gv_->unit_action_view();
  unit_action_view->SetUnit(unit_);
  unit_action_view->SetCoords(layout::CalcPositionNearUnit(unit_action_view->GetFrameSize(), gv_->GetFrameSize(),
                                                           gv_->GetCameraCoords(), unit_->GetPosition()));
  unit_action_view->visible(true);
}

void StateUIAction::Exit() { gv_->unit_action_view()->visible(false); }

void StateUIAction::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(unit_->GetPosition(), 4);
}

bool StateUIAction::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

// StateUIMagicSelection

StateUIMagicSelection::StateUIMagicSelection(StateUI::Base base, core::Unit* unit) : StateUI(base), unit_(unit) {}

void StateUIMagicSelection::Enter() {
  auto           magic_list = std::make_shared<core::MagicList>(game_->GetMagicManager(), unit_);
  MagicListView* mlv        = gv_->magic_list_view();
  mlv->SetUnitAndMagicList(unit_, magic_list);
  mlv->SetCoords(layout::CalcPositionNearUnit(mlv->GetFrameSize(), gv_->GetFrameSize(), gv_->GetCameraCoords(),
                                              unit_->GetPosition()));
  mlv->visible(true);
}

void StateUIMagicSelection::Exit() { gv_->magic_list_view()->visible(false); }

void StateUIMagicSelection::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(unit_->GetPosition(), 4);
}

bool StateUIMagicSelection::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

// StateUINextTurn

StateUINextTurn::StateUINextTurn(StateUI::Base base) : StateUI(base), frames_(-1) {}

void StateUINextTurn::Update() {
  frames_++;
  if (frames_ >= 15 /*60*2*/) {
    gv_->PopUIState();
  }
}

void StateUINextTurn::Render(Drawer*) {}

void StateUINextTurn::Enter() {
  ModalDialogView* modal_dialog_view = gv_->dialog_view();
  modal_dialog_view->SetText("End Turn");
  modal_dialog_view->visible(true);
}

void StateUINextTurn::Exit() {
  gv_->dialog_view()->visible(false);

  GameView*   gv   = gv_;
  core::Game* game = game_;
  gv_->NextFrame([=]() {
    ControlView* control_view = gv->control_view();
    control_view->SetTurnText(game->GetTurnCurrent(), game->GetTurnLimit());
    control_view->SetEndTurnVisible(game->IsUserTurn());
  });
}

// StateUISpeak

StateUISpeak::StateUISpeak(StateUI::Base base, core::Unit* unit, const string& words)
    : StateUI(base), unit_(unit), words_(words) {}

void StateUISpeak::Enter() {
  auto unit_dialog_view         = gv_->unit_dialog_view();
  auto unit_dialog_view_wrapper = gv_->unit_dialog_view_wrapper();
  unit_dialog_view->SetUnit(unit_);
  unit_dialog_view->SetText(words_);
  unit_dialog_view->SetCoords(layout::CalcPositionNearUnit(gv_->unit_dialog_view()->GetFrameSize(), gv_->GetFrameSize(),
                                                           gv_->GetCameraCoords(), unit_->GetPosition()));
  unit_dialog_view_wrapper->visible(true);
}

void StateUISpeak::Exit() { gv_->unit_dialog_view_wrapper()->visible(false); }

void StateUISpeak::Update() {
  if (!gv_->unit_dialog_view_wrapper()->visible()) {
    gv_->PopUIState();
  }
}

bool StateUISpeak::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    //    gv_->PopUIState();
    return true;
  }
  return true;
}

// StateUIEnd

StateUIEnd::StateUIEnd(StateUI::Base base, bool is_victory) : StateUI(base), is_victory_(is_victory) {}

void StateUIEnd::Enter() {
  ModalDialogView* modal_dialog_view = gv_->dialog_view();
  modal_dialog_view->SetText(is_victory_ ? "Victory" : "Defeat");
  modal_dialog_view->visible(true);
}

void StateUIEnd::Update() {
  ModalDialogView* modal_dialog_view = gv_->dialog_view();
  if (modal_dialog_view->visible() == false) {
    // Advance to next stage unless we're in the last stage
    gv_->EndGame();
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

#include "state_ui.h"
#include "root_view.h"
#include "app.h"
#include "misc.h"
#include "texture_manager.h"
#include "texture_animator.h"
#include "core/game.h"
#include "core/cmd.h"
#include "core/formulae.h"
#include "core/map.h"
#include "core/unit.h"
#include "core/magic.h"
#include "util/path_tree.h"
#include "view/ui/terrain_info_view.h"
#include "view/ui/unit_info_view.h"
#include "view/ui/modal_dialog_view.h"
#include "view/drawer.h"

// StateUI

StateUI::StateUI(Base base)
  : game_(base.game), rv_(base.rv) {
}

// StateUIMain

StateUIMain::StateUIMain(StateUI::Base base) : StateUI(base) {
}

// StateUIDoCmd

StateUIDoCmd::StateUIDoCmd(StateUI::Base base) : StateUI(base) {
}

void StateUIDoCmd::Enter() {
  while (game_->HasPendingCmd()) {
    StateUI* state = GenerateNextCmdUIState();
    if (state != nullptr) {
      rv_->PushUIState(state);
      game_->DoPendingCmd();
      break;
    } else {
      game_->DoPendingCmd();
    }
  }
}

// Generate corresponding StateUI for next Cmd
// Returns nullptr when no UI needed
StateUI* StateUIDoCmd::GenerateNextCmdUIState() {
  ASSERT(game_->HasPendingCmd());
  const Cmd* cmd = game_->GetNextCmdConst();
  StateUI* no_state_ui = nullptr;

#define DYNAMIC_CAST_CHECK(type) \
  dynamic_cast<const type*>(cmd); \
  ASSERT(c != nullptr);

  switch (cmd->GetOp()) {
    case Cmd::Op::kCmdAction:
    case Cmd::Op::kCmdPlayAI:
    case Cmd::Op::kCmdGameWin:
      return no_state_ui;

    case Cmd::Op::kCmdHit: {
      const CmdHit* c = DYNAMIC_CAST_CHECK(CmdHit);
      if (c->IsBasicAttack()) {
        return new StateUIAttack(WrapBase(),
                                 c->GetUnitAtk(),
                                 c->GetUnitDef(),
                                 true,
                                 c->GetHitType() == CmdHit::HitType::kCritical,
                                 c->GetDamage());
      } else {
        ASSERT(c->IsMagic());
        return new StateUIMagic(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), c->GetMagic(), true, c->GetDamage());
      }
    }
    case Cmd::Op::kCmdMiss: {
      const CmdMiss* c = DYNAMIC_CAST_CHECK(CmdMiss);
      if (c->IsBasicAttack()) {
        return new StateUIAttack(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), false, false, 0);
      } else {
        ASSERT(c->IsMagic());
        return new StateUIMagic(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), c->GetMagic(), false, 0);
      }
    }
    case Cmd::Op::kCmdMove: {
      const CmdMove* c = DYNAMIC_CAST_CHECK(CmdMove);
      StateUIMoving::Flag flag = StateUIMoving::Flag::kNone;
      return new StateUIMoving(WrapBase(), c->GetUnit(), c->GetDest(), flag);
    }
    case Cmd::Op::kCmdKilled: {
      const CmdKilled* c = DYNAMIC_CAST_CHECK(CmdKilled);
      return new StateUIKilled(WrapBase(), c->GetUnit());
    }
    case Cmd::Op::kCmdEndTurn:
      return new StateUINextTurn(WrapBase());
      
    case Cmd::Op::kCmdSpeak: {
      const CmdSpeak* c = DYNAMIC_CAST_CHECK(CmdSpeak);
      return new StateUISpeak(WrapBase(), c->GetUnit(), c->GetWords());
    }
    case Cmd::Op::kCmdGameEnd: {
      const CmdGameEnd* c = DYNAMIC_CAST_CHECK(CmdGameEnd);
      return new StateUIEnd(WrapBase());
    }

    default:
//      UNREACHABLE("Unknown type of Cmd");
      return no_state_ui;
  }

  return no_state_ui;
#undef DYNAMIC_CAST_CHECK
}

void StateUIDoCmd::Exit() {
}

void StateUIDoCmd::Render(Drawer*) {
}

void StateUIDoCmd::Update() {
  if (!game_->HasPendingCmd()) {
    rv_->PopUIState();
  }
}

bool StateUIDoCmd::OnMouseButtonEvent(const MouseButtonEvent) {
  return false;
}

bool StateUIDoCmd::OnMouseMotionEvent(const MouseMotionEvent) {
  return true;
}

// StateUIOperable

StateUIOperable::StateUIOperable(Base base)
    : StateUI(base),
      cursor_cell_(-1, -1) {
  ClearScrolls();
}

void StateUIOperable::Update() {
  Map* map = game_->GetMap();
  rv_->SetUnitInfoViewVisible(false);
  rv_->SetUnitViewVisible(false);
  if (map->UnitInCell(cursor_cell_)) {
    Unit* unit = map->GetUnit(cursor_cell_);
    rv_->SetUnitInfoViewUnitTerrainInfo(map->GetCell(cursor_cell_)); // TODO Refactor : remove dependency with Unit
    rv_->SetUnitInfoViewVisible(true);
    rv_->SetUnitViewUnit(unit); // TODO Refactor : remove dependency with Unit
    rv_->SetUnitViewVisible(true);
  }

  // Update camera coords
  static const int kScrollAmount = 8;
  if (IsScrollLeft()) rv_->MoveCameraX(-kScrollAmount);
  if (IsScrollRight()) rv_->MoveCameraX(kScrollAmount);
  if (IsScrollUp()) rv_->MoveCameraY(-kScrollAmount);
  if (IsScrollDown()) rv_->MoveCameraY(kScrollAmount);
//  cursor_cell_ = (rv_->GetMouseCoords() + rv_->GetCameraCoords()) / App::kBlockSize;
}

void StateUIOperable::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(255, 255, 255, 128));
  drawer->BorderCell(cursor_cell_, 4);
}

bool StateUIOperable::OnMouseMotionEvent(const MouseMotionEvent e) {
  cursor_cell_ = (rv_->GetMouseCoords() + rv_->GetCameraCoords()) / App::kBlockSize;

  // mouse scroll
  int window_width  = rv_->GetFrameSize().x;
  int window_height = rv_->GetFrameSize().y;
  const int kScrollRange = App::kBlockSize;
  const int kLeftScroll  = kScrollRange;
  const int kRightScroll = window_width - kScrollRange;
  const int kUpScroll    = kScrollRange;
  const int kDownScroll  = window_height - kScrollRange;

  ClearScrolls();
  Vec2D mouse_coords = rv_->GetMouseCoords();
  if (mouse_coords.x < kLeftScroll) SetScrollLeft();
  if (mouse_coords.x > kRightScroll) SetScrollRight();
  if (mouse_coords.y < kUpScroll) SetScrollUp();
  if (mouse_coords.y > kDownScroll) SetScrollDown();
  return true;
}

void StateUIOperable::ClearScrolls() {
  scroll_left_ = scroll_right_ = scroll_up_ = scroll_down_ = false;
}

void StateUIOperable::SetScrollLeft() {
  scroll_left_ = true;
}

void StateUIOperable::SetScrollRight() {
  scroll_right_ = true;
}

void StateUIOperable::SetScrollUp() {
  scroll_up_ = true;
}

void StateUIOperable::SetScrollDown() {
  scroll_down_ = true;
}

bool StateUIOperable::IsScrollLeft() {
  return scroll_left_;
}

bool StateUIOperable::IsScrollRight() {
  return scroll_right_;
}

bool StateUIOperable::IsScrollUp() {
  return scroll_up_;
}

bool StateUIOperable::IsScrollDown() {
  return scroll_down_;
}

// StateUIView

StateUIView::StateUIView(Base base) : StateUIOperable(base) {
}

void StateUIView::Update() {
  StateUIOperable::Update();

  if (game_->IsAITurn()) {
    game_->PushCmd(unique_ptr<CmdPlayAI>(new CmdPlayAI()));
  }

  if (game_->HasPendingCmd()) {
    rv_->PushUIState(new StateUIDoCmd({game_, rv_}));
  }
}

bool StateUIView::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    Vec2D pos = GetCursorCell();
    Map* map = game_->GetMap();
    if (map->UnitInCell(pos)) {
      Unit* unit = map->GetUnit(pos);
      if (game_->IsCurrentTurn(unit)) {
        if (unit->IsDoneAction()) {
          LOG_INFO("The chosen unit is already done his action");
        } else {
          PathTree* pathtree = map->FindMovablePath(unit);
          rv_->PushUIState(new StateUIUnitSelected(WrapBase(), unit, pathtree));
        }
      } else {
        // TODO alert this is not your unit
      }
    } else {
      rv_->PushUIState(new StateUIEmptySelected(WrapBase(), pos));
    }
  }
  return true;
}

bool StateUIView::OnMouseMotionEvent(const MouseMotionEvent e) {
  return StateUIOperable::OnMouseMotionEvent(e);
}

// StateUIUnitSelected

StateUIUnitSelected::StateUIUnitSelected(StateUI::Base base,
                                         Unit* unit,
                                         PathTree* pathtree)
    : StateUIOperable(base), unit_(unit), pathtree_(pathtree), origin_coords_(unit_->GetCoords()) {
}

StateUIUnitSelected::~StateUIUnitSelected() {
  if (pathtree_ != nullptr)
    delete pathtree_;
}

void StateUIUnitSelected::Enter() {
  game_->MoveUnit(unit_, origin_coords_);
  rv_->CenterCamera(unit_->GetCoords() * App::kBlockSize + (App::kBlockSize / 2));
}

void StateUIUnitSelected::Exit() {
}

void StateUIUnitSelected::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(unit_->GetCoords(), 4);

  std::vector<Vec2D> movable_cells = pathtree_->GetNodeList();
  drawer->SetDrawColor(Color(0, 0, 192, 96));
  Map* map = game_->GetMap();
  for (auto cell : movable_cells) {
    if (!map->UnitInCell(cell)) {
      drawer->FillCell(cell);
    }
  }

  StateUIOperable::Render(drawer);
}

void StateUIUnitSelected::Update() {
  StateUIOperable::Update();

  rv_->SetUnitViewUnit(unit_);
  rv_->SetUnitViewVisible(true);
}

bool StateUIUnitSelected::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    std::vector<Vec2D> movable_cells = pathtree_->GetNodeList();
    Vec2D pos = GetCursorCell();
    Map* map = game_->GetMap();
    if (map->UnitInCell(pos) && map->GetUnit(pos) != unit_) {
      // XXX Other unit clicked
    } else if (std::find(movable_cells.begin(), movable_cells.end(), pos) !=
               movable_cells.end()) {
      rv_->PushUIState(new StateUIMoving({game_, rv_}, unit_, GetPathToRoot(pos)));
    } else {
//      rv_->ChangeStateUI(new StateUIView(game_, rv_));
    }
  }
  else if (e.IsRightButtonUp()) {
    rv_->PopUIState();
  }
  return true;
}

std::vector<Vec2D> StateUIUnitSelected::GetPathToRoot(Vec2D pos) {
  return pathtree_->GetPathToRoot(pos);
}


// StateUIMoving

StateUIMoving::StateUIMoving(StateUI::Base base,
                             Unit*         unit,
                             Vec2D         dest,
                             Flag          flag)
    : StateUI(base), unit_(unit), dest_(dest), path_(), frames_(-1), flag_(flag) {
}

StateUIMoving::StateUIMoving(StateUI::Base        base,
                             Unit*                unit,
                             const vector<Vec2D>& path,
                             Flag                 flag)
    : StateUI(base), unit_(unit), dest_(), path_(path), frames_(-1), flag_(flag) {
  ASSERT(!path_.empty());
  dest_ = path_[0];
}

void StateUIMoving::Enter() {
  unit_->SetNoRender(true);

  if (path_.empty()) {
    // Lazy path generation
    Map* m = game_->GetMap();
    path_ = m->FindPathTo(unit_, dest_);
    ASSERT(dest_ == path_[0]);
  }
}

void StateUIMoving::Exit() {
  unit_->SetNoRender(false);
}

void StateUIMoving::Update() {
  int path_idx = CalcPathIdx();
  if (path_idx == 0) { // Arrived at the destination
    ASSERT(dest_ == path_[0]);
    game_->MoveUnit(unit_, dest_);
    if (path_.size() > 1) {
      Direction dir = Vec2DRelativePosition(path_[1], path_[0]);
      unit_->SetDirection(dir); // Do this here?
    }
    if (flag_ == Flag::kInputActNext) {
      rv_->ChangeUIState(new StateUIAction(WrapBase(), unit_));
    } else {
      rv_->PopUIState();
    }
  }
  frames_++;
}

void StateUIMoving::Render(Drawer* drawer) {
  int path_idx = CalcPathIdx();

  int frames_current = frames_ % kFramesPerCell;
  int sprite_no = frames_current / (kFramesPerCell / 2);
  float percentage = frames_current / (float)kFramesPerCell;
  Direction dir = Vec2DRelativePosition(path_[path_idx], path_[path_idx - 1]);
  Vec2D diff = path_[path_idx - 1] - path_[path_idx];
  Vec2D diff_pos = diff * (percentage * (float)App::kBlockSize);
  drawer->CopySprite(unit_->GetBitmapPath(),
                     kSpriteMove,
                     dir,
                     sprite_no,
                     {kEffectNone, 0},
                     path_[path_idx],
                     diff_pos);
  rv_->CenterCamera(path_[path_idx] * App::kBlockSize + diff_pos + (App::kBlockSize / 2));
}

// StateUIMagic

StateUIMagic::StateUIMagic(StateUI::Base base, Unit* atk, Unit* def, Magic* magic, bool hit, int damage)
    : StateUI(base),
      atk_(atk),
      def_(def),
      magic_(magic),
      hit_(hit),
      damage_(damage),
      animator_(nullptr) {
}

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

/*
  Vec2D unit_pos = atk_->GetCoords();
  Vec2D def_pos = def_->GetCoords();
  Direction dir = Vec2DRelativePosition(unit_pos, def_pos);
  atk_->SetDirection(dir);
  def_->SetDirection(OppositeDirection(dir));
*/
}

void StateUIMagic::Render(Drawer* drawer) {
  if (animator_ == NULL) {
    TextureManager* tm = drawer->GetTextureManager();
    Texture* texture = tm->FetchTexture("magic/" + magic_->GetId() + ".bmp");
    texture->SetAlpha(160); // FIXME non-fixed alpha value
    animator_ = new TextureAnimator(texture, kFramesPerCut);
  }

  Vec2D unit_pos = atk_->GetCoords();
  Vec2D def_pos = def_->GetCoords();
  Direction dir = Vec2DRelativePosition(unit_pos, def_pos);
  drawer->CopySprite(atk_->GetBitmapPath(),
                     kSpriteStand,
                     dir,
                     0,
                     {kEffectNone, 0},
                     unit_pos);
  const SpriteType target_sprite_success = magic_->IsTypeDeal() ? kSpriteDamaged : kSpriteBuff; // This condition may not be accurate
  const SpriteType target_sprite = hit_ ? target_sprite_success : kSpriteBlocked;

  drawer->CopySprite(def_->GetBitmapPath(),
                     target_sprite,
                     OppositeDirection(dir),
                     0,
                     {kEffectNone, 0},
                     def_pos);

  Rect src_rect = animator_->GetCurrentCutRect();
  drawer->CopyTextureToCell(animator_->GetTexture(), &src_rect, def_pos);
}

void StateUIMagic::Update() {
  if (animator_) {
    animator_->NextFrame();
    if (animator_->DoneAnimate()) {
      rv_->InitUIStateMachine();
    }
  }
}

// StateUIKilled

StateUIKilled::StateUIKilled(StateUI::Base base, Unit* unit)
    : StateUI(base),
      unit_(unit),
      frames_(0) {
}

void StateUIKilled::Enter() {
  unit_->SetNoRender(true);
  Misc::SetShowCursor(false);
}

void StateUIKilled::Exit() {
  unit_->SetNoRender(false);
  Misc::SetShowCursor(true);
}

void StateUIKilled::Render(Drawer* drawer) {
  const int wait = 20;
  const int hold = 20;
  int progress = 0;
  if (frames_ >= wait) {
    int f = frames_ - wait;
    if (f > hold) progress = (f - hold) * 255 / (kStateDuration - hold);
  }
  drawer->CopySprite(unit_->GetBitmapPath(),
                     kSpriteLowHP,
                     kDirNone,
                     0,
                     {kEffectShade, progress},
                     unit_->GetCoords());
}

void StateUIKilled::Update() {
  frames_++;
  if (frames_ == kStateDuration) {
    rv_->PopUIState();
  }
}

// StateUIEmptySelected

StateUIEmptySelected::StateUIEmptySelected(Base base, Vec2D coords)
    : StateUI(base), coords_(coords), terrain_info_view_(NULL) {
}

void StateUIEmptySelected::Enter() {
//  SelectCell(coords_);
  Map* map = game_->GetMap();
  Terrain* terrain = map->GetTerrain(coords_);
  Rect frame = LayoutHelper::CalcPosition({800, 600}, // FIXME hardcoded window size
                                          {200, 100},
                                          LayoutHelper::kAlignLftBot,
                                          LayoutHelper::kDefaultSpace);
  terrain_info_view_ = new TerrainInfoView(&frame, coords_, terrain);
}

void StateUIEmptySelected::Exit() {
//  DeselectCell();
  ASSERT(terrain_info_view_ != NULL);
  terrain_info_view_ = NULL;
  delete terrain_info_view_;
}

void StateUIEmptySelected::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(coords_, 4);

  terrain_info_view_->Render(drawer);
}

bool StateUIEmptySelected::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (e.IsLeftButtonUp() || e.IsRightButtonUp()) {
    rv_->PopUIState();
  }
  return true;
}

// StateUIAttack

StateUIAttack::StateUIAttack(StateUI::Base base,
                             Unit* atk,
                             Unit* def,
                             bool hit,
                             bool critical,
                             int damage)
    : StateUI(base), atk_(atk), def_(def), hit_(hit), critical_(critical), damage_(damage), frames_(0) {
}

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
    int no;
    int offset;
  };

  static const CutInfo kCutInfoAtk[kNumCuts] = {
    {kSpriteStand,  0, 0},
    {kSpriteStand,  0, 0},
    {kSpriteAttack, 0, 0},
    {kSpriteAttack, 0, 0},
    {kSpriteAttack, 0, 0},
    {kSpriteAttack, 1, 0},
    {kSpriteAttack, 2, 8},
    {kSpriteAttack, 3, 8},
    {kSpriteAttack, 3, 8},
    {kSpriteAttack, 3, 8}
  };

  static const CutInfo kCutInfoDefDamaged[kNumCuts] = {
    {kSpriteStand,   0, 0},
    {kSpriteStand,   0, 0},
    {kSpriteStand,   0, 0},
    {kSpriteStand,   0, 0},
    {kSpriteStand,   0, 0},
    {kSpriteStand,   0, 0},
    {kSpriteDamaged, 0, 0},
    {kSpriteDamaged, 0, 0},
    {kSpriteDamaged, 0, 0},
    {kSpriteDamaged, 0, 0}
  };

  static const CutInfo kCutInfoDefBlocked[kNumCuts] = {
    {kSpriteStand,   0,  0},
    {kSpriteStand,   0,  0},
    {kSpriteStand,   0,  0},
    {kSpriteStand,   0,  0},
    {kSpriteStand,   0,  0},
    {kSpriteStand,   0,  0},
    {kSpriteBlocked, 0, -4},
    {kSpriteBlocked, 0, -4},
    {kSpriteBlocked, 0, -4},
    {kSpriteBlocked, 0, -4}
  };

  int cut_no = frames_ / kFramesPerCut;
  const CutInfo* cut_atk = &kCutInfoAtk[cut_no];
  const CutInfo* cut_def = hit_ ? &kCutInfoDefDamaged[cut_no] : &kCutInfoDefBlocked[cut_no];
  Vec2D atk_pos = atk_->GetCoords();
  Vec2D def_pos = def_->GetCoords();
  Vec2D unit_offset = GenerateVec2DOffset(atk_->GetDirection(), cut_atk->offset);
  Vec2D def_offset = GenerateVec2DOffset(def_->GetDirection(), cut_def->offset);
  Direction dir = Vec2DRelativePosition(atk_pos, def_pos);

  SpriteEffect sprite_effect = {kEffectNone, 0};
  if (critical_) {
    const int maxp = 224;
    const int unit_frames = kStateDuration / 5;
    int progress = 0;
    if (frames_ < unit_frames) {
      progress = frames_ * maxp / unit_frames;
    } else if (frames_ < unit_frames * 2) {
      progress = maxp;
    } else if (frames_ < unit_frames * 3) {
      progress = maxp - (frames_ - unit_frames * 2) * maxp / unit_frames;
    }
    sprite_effect = {kEffectBright, progress};
  }

  if (atk_->GetCoords().y > def_->GetCoords().y) {
    drawer->CopySprite(def_->GetBitmapPath(),
                       cut_def->sprite,
                       OppositeDirection(dir),
                       cut_def->no,
                       {kEffectNone, 0},
                       def_pos,
                       def_offset);
    drawer->CopySprite(atk_->GetBitmapPath(),
                       cut_atk->sprite,
                       dir,
                       cut_atk->no,
                       sprite_effect,
                       atk_pos,
                       unit_offset);
  } else {
    drawer->CopySprite(atk_->GetBitmapPath(),
                       cut_atk->sprite,
                       dir,
                       cut_atk->no,
                       sprite_effect,
                       atk_pos,
                       unit_offset);
    drawer->CopySprite(def_->GetBitmapPath(),
                       cut_def->sprite,
                       OppositeDirection(dir),
                       cut_def->no,
                       {kEffectNone, 0},
                       def_pos,
                       def_offset);
  }

  // Render Damage Text
  if (frames_ >= 5 * kFramesPerCut) {
    const int text_size = 13;
    Vec2D damage_text_pos = def_pos * App::kBlockSize;
    string damage_text;
    if (!hit_) {
      damage_text = "miss";
    } else {
      damage_text = std::to_string(damage_);
    }

    // FIXME inefficient border effect
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        Vec2D border_pos = {i, j};
        drawer->DrawText(damage_text, text_size, COLOR_WHITE, damage_text_pos + border_pos);
      }
    }
    drawer->DrawText(damage_text, text_size, {228, 32, 32, 255}, damage_text_pos);
  }
}

void StateUIAttack::Update() {
  frames_++;
  if (frames_ >= kNumCuts * kFramesPerCut) {
    rv_->PopUIState();
  }
}

// StateUIAction

StateUIAction::StateUIAction(StateUI::Base base,
                             Unit* unit,
                             const string& magic_id)
    : StateUIOperable(base),
      unit_(unit),
      target_info_view_(NULL),
      magic_id_(magic_id),
      range_itr_(NULL),
      is_basic_attack_(true) {
  if (!magic_id.compare("basic_attack")) {
    is_basic_attack_ = true;
    range_itr_ = unit_->GetAttackRange();
  } else {
    is_basic_attack_ = false;
    Magic* magic = game_->GetMagic(magic_id_);
    range_itr_ = magic->GetRange(); // FIXME
  }
  Rect frame = LayoutHelper::CalcPosition(rv_->GetFrameSize(),
                                          {200, 100},
                                          LayoutHelper::kAlignLftBot,
                                          LayoutHelper::kDefaultSpace);
  frame.Move(frame.GetW() + LayoutHelper::kDefaultSpace, 0);
  target_info_view_ = new UnitInfoView(&frame, unit_);
  target_info_view_->SetVisible(false);
}

StateUIAction::~StateUIAction() {
  delete target_info_view_;
}

void StateUIAction::Enter() {
  if (is_basic_attack_) {
    rv_->ShowMagicListView(unit_);
  }
}

void StateUIAction::Exit() {
  if (is_basic_attack_) {
    rv_->HideMagicListView();
  }
}

void StateUIAction::Render(Drawer* drawer) {

  // Show Attack Range
  ASSERT(range_itr_ != NULL);
  Vec2D* range_itr = range_itr_;
  Vec2D pos = unit_->GetCoords();
  const Vec2D nil = {0, 0};
  for (; *range_itr != nil; range_itr++) {
    Vec2D cpos = pos + *range_itr;
    if (!game_->IsValidCoords(cpos))
      continue;
    drawer->SetDrawColor(Color(255, 64, 64, 128));
    drawer->FillCell(cpos);
  }

  StateUIOperable::Render(drawer);
  target_info_view_->Render(drawer);
}

void StateUIAction::Update() {
  StateUIOperable::Update();

  rv_->SetUnitViewUnit(unit_);
  rv_->SetUnitViewVisible(true);
}

bool StateUIAction::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    Vec2D map_pos = GetCursorCell();
    Map* map = game_->GetMap();
    if (map->UnitInCell(map_pos)) {
      Unit* atk = unit_;
      Unit* def = map->GetUnit(map_pos);
      if (is_basic_attack_) {
        if (atk->IsInRange(map_pos)) {
          if (atk->IsHostile(def)) {
            unique_ptr<CmdAction> action(new CmdAction());
            action->SetCmdMove(unique_ptr<CmdMove>(new CmdMove(atk, unit_->GetCoords())));
            action->SetCmdAct(unique_ptr<CmdBasicAttack>(new CmdBasicAttack(atk, def, CmdBasicAttack::Type::kActive)));
            game_->PushCmd(std::move(action));
            rv_->InitUIStateMachine();
          } else {
            LOG_INFO("Cannot attack the unit. It is not enemy");
          }
        }
      } else {
        Magic* magic = game_->GetMagic(magic_id_);
        if (atk->IsInRange(map_pos, magic->GetRange())) {
          if (atk->IsHostile(def) == magic->GetIsTargetEnemy()) {
            unique_ptr<CmdAction> action(new CmdAction());
            action->SetCmdMove(unique_ptr<CmdMove>(new CmdMove(atk, unit_->GetCoords())));
            action->SetCmdAct(unique_ptr<CmdMagic>(new CmdMagic(atk, def, magic)));
            game_->PushCmd(std::move(action));
            rv_->InitUIStateMachine();
          } else {
            // TODO alert
            LOG_INFO("Cannot perform magic to the unit chosen.");
          }
        }
      }
    }
  } else if (e.IsRightButtonUp()) {
    rv_->PopUIState();
  }
  return true;
}

bool StateUIAction::OnMouseMotionEvent(const MouseMotionEvent e) {
  StateUIOperable::OnMouseMotionEvent(e);

  Map* map = game_->GetMap();
  Vec2D cursor_cell = GetCursorCell();
  bool info_visible = false;
  if (map->UnitInCell(cursor_cell)) {
    Unit* unit_target = map->GetUnit(cursor_cell);
    bool hostile = unit_->IsHostile(unit_target);
    if (hostile) {
      int damage = is_basic_attack_ ? Formulae::ComputeBasicAttackDamage(map, unit_, unit_target) :
                                      Formulae::ComputeMagicDamage(map, unit_, unit_target);
      int accuracy = is_basic_attack_ ? Formulae::ComputeBasicAttackAccuracy(unit_, unit_target) :
                                        Formulae::ComputeMagicAccuracy(unit_, unit_target);

      target_info_view_->SetUnitAttackInfo(unit_target, accuracy, damage);
    }
    info_visible = hostile;
  }
  target_info_view_->SetVisible(info_visible);
  return true;
}

// StateUINextTurn

StateUINextTurn::StateUINextTurn(StateUI::Base base)
    : StateUI(base), frames_(-1) {
}

void StateUINextTurn::Update() {
  frames_++;
  if (frames_ >= 15/*60*2*/) {
    rv_->PopUIState();
  }
}

void StateUINextTurn::Render(Drawer* drawer) {
}

void StateUINextTurn::Enter() {
  Rect frame = {0, 0, 800, 600};
  rv_->SetDialogViewText("End Turn");
  rv_->SetDialogViewVisible(true);
}

void StateUINextTurn::Exit() {
  rv_->SetDialogViewVisible(false);
}

// StateUISpeak

StateUISpeak::StateUISpeak(StateUI::Base base, Unit* unit, const string& words)
    : StateUI(base), unit_(unit), words_(words) {
}

void StateUISpeak::Enter() {
  rv_->SetUnitDialogViewUnit(unit_);
  rv_->SetUnitDialogViewText(words_);
  rv_->SetUnitDialogViewVisible(true);
}

void StateUISpeak::Exit() {
  rv_->SetUnitDialogViewVisible(false);
}

void StateUISpeak::Update() {
  if (!rv_->IsUnitDialogViewVisible()) {
    rv_->PopUIState();
  }
}

bool StateUISpeak::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
//    rv_->PopUIState();
    return true;
  }
  return true;
}

// StateUIEnd

StateUIEnd::StateUIEnd(StateUI::Base base) : StateUI(base) {
}

void StateUIEnd::Enter() {
  rv_->EndGame();
}


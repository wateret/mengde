#include "state_ui.h"

#include <boost/format.hpp>

#include "config.h"
#include "control_view.h"
#include "core/attack_range.h"
#include "core/cmd.h"
#include "core/formulae.h"
#include "core/magic.h"
#include "core/magic_list.h"
#include "core/map.h"
#include "core/path_tree.h"
#include "core/stage.h"
#include "core/unit.h"
#include "core/user_interface.h"
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
#include "state_ui_factory.h"
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

StateUI::StateUI(Base base) : gi_(base.gi), gv_(base.gv) {}

// StateUIMain

StateUIMain::StateUIMain(StateUI::Base base) : StateUI(base) {}

// StateUIDoCmd

StateUIDoCmd::StateUIDoCmd(StateUI::Base base) : StateUI(base), cmd_to_do_(false) {}

void StateUIDoCmd::Enter() {
  // FIXME Avoid flag based operations
  //       Can we do the command before StateUIView?
  if (cmd_to_do_) {
    // Run reserved command run
    ASSERT(gi_->HasNextCmd());
    gi_->DoNextCmd();
    cmd_to_do_ = false;
  }
  while (gi_->HasNextCmd()) {
    StateUI* state = StateUIFactory{gi_, gv_}.GenerateNext();
    if (state != nullptr) {
      gv_->PushUIState(state);
      // Reserve doing the command. Cmd will be run when coming back(pop from the pushed state) to this State
      cmd_to_do_ = true;
      break;
    } else {
      // If we do not have UIState for the command do it immediately
      gi_->DoNextCmd();
    }
  }
}

void StateUIDoCmd::Exit() {}

void StateUIDoCmd::Render(Drawer*) {}

void StateUIDoCmd::Update() {
  if (!gi_->HasNextCmd()) {
    gv_->PopUIState();
  }
}

bool StateUIDoCmd::OnMouseButtonEvent(const foundation::MouseButtonEvent&) { return false; }

bool StateUIDoCmd::OnMouseMotionEvent(const foundation::MouseMotionEvent&) { return true; }

// StateUIOperable

StateUIOperable::StateUIOperable(Base base) : StateUI(base), cursor_cell_(0, 0) { ClearScrolls(); }

void StateUIOperable::Enter() {}

void StateUIOperable::Exit() { gv_->unit_tooltip_view()->visible(false); }

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

bool StateUIOperable::OnMouseMotionEvent(const foundation::MouseMotionEvent& e) {
  if (e.IsMotionOver()) {
    // mouse scroll
    const int window_width = gv_->GetFrameSize().x;
    const int window_height = gv_->GetFrameSize().y;
    const int kScrollRange = config::kBlockSize;
    const int kLeftScroll = kScrollRange;
    const int kRightScroll = window_width - kScrollRange;
    const int kUpScroll = kScrollRange;
    const int kDownScroll = window_height - kScrollRange;

    ClearScrolls();
    Vec2D mouse_coords = gv_->GetMouseCoords();
    if (mouse_coords.x < kLeftScroll) SetScrollLeft();
    if (mouse_coords.x > kRightScroll) SetScrollRight();
    if (mouse_coords.y < kUpScroll) SetScrollUp();
    if (mouse_coords.y > kDownScroll) SetScrollDown();
  }
  return true;
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

StateUIView::StateUIView(Base base) : StateUIOperable(base), units_(gi_->QueryUnits()) {}

void StateUIView::Update() {
  StateUIOperable::Update();

  auto unit_tooltip_view = gv_->unit_tooltip_view();
  auto unit_view = gv_->unit_view();
  const core::Unit* unit = gi_->GetUnit(cursor_cell_);
  if (unit) {
    const core::Cell* cell = gi_->GetCell(cursor_cell_);
    unit_tooltip_view->SetUnitTerrainInfo(cell, unit);
    unit_tooltip_view->SetCoordsByUnitCoords(unit->position(), gv_->GetCameraCoords(), gv_->GetFrameSize());
    unit_view->SetUnit(unit);

    unit_tooltip_view->visible(true);
  } else {
    unit_tooltip_view->visible(false);
  }

  if (gi_->IsAITurn()) {
    gi_->PushPlayAI();
  }

  if (gi_->HasNextCmd()) {
    gv_->PushUIState(new StateUIDoCmd(WrapBase()));
  }
}

void StateUIView::Enter() {
  StateUIOperable::Enter();
  units_ = gi_->QueryUnits();
}

void StateUIView::Exit() { StateUIOperable::Exit(); }

bool StateUIView::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  if (e.IsLeftButtonUp()) {
    Vec2D pos = GetCursorCell();
    auto unit_id = units_.FindByPos(pos);
    if (unit_id) {
      gv_->PushUIState(new StateUIUnitSelected(WrapBase(), unit_id));
    } else {
      LOG_DEBUG("Not valid unit");

      if (true) {  // TODO Check if the position is empty
        gv_->PushUIState(new StateUIEmptySelected(WrapBase(), pos));
      } else {
        // TODO Implement for 2 cases (Need UserInterface support)
        //        1. Current turn unit but done action
        //          - Show a message (TBD)
        //        2. Other's unit
        //          - Show a message (TBD)
      }
    }
  } else if (e.IsRightButtonUp()) {
    // TODO Enable UnitListView
#if 0
    Vec2D pos = GetCursorCell();
    auto unit_id_opt = units_.FindByPos(pos);
    if (unit_id_opt) {
      uint32_t unit_id = unit_id_opt.get();
      gv_->unit_list_view()->SetUnit(gi_->GetUnit(unit_id));
      gv_->unit_list_view()->visible(true);
    }
#endif
  }
  return true;
}

bool StateUIView::OnMouseMotionEvent(const foundation::MouseMotionEvent& e) {
  return StateUIOperable::OnMouseMotionEvent(e);
}

// StateUIUnitSelected

StateUIUnitSelected::StateUIUnitSelected(StateUI::Base base, const core::UnitKey& unit_key)
    : StateUIOperable(base), unit_key_(unit_key), moves_(gi_->QueryMoves(unit_key_)) {
  key_mapper_.Set(SDLK_ESCAPE, KeyCmd::kBack);

  const core::Unit* unit = gi_->GetUnit(unit_key_);
  origin_coords_ = unit->position();
}

void StateUIUnitSelected::Enter() {}

void StateUIUnitSelected::Exit() {}

void StateUIUnitSelected::Render(Drawer* drawer) {
  const core::Unit* unit = gi_->GetUnit(unit_key_);

  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(unit->position(), 4);

  drawer->SetDrawColor(Color(0, 0, 192, 96));
  for (auto pos : moves_.moves()) {
    if (gi_->GetUnit(pos) == nullptr) {
      drawer->FillCell(pos);
    }
  }

  StateUIOperable::Render(drawer);
}

void StateUIUnitSelected::Update() {
  StateUIOperable::Update();

  auto unit_view = gv_->unit_view();
  unit_view->SetUnit(gi_->GetUnit(unit_key_));
  unit_view->visible(true);
}

bool StateUIUnitSelected::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  if (e.IsLeftButtonUp()) {
    // TODO Provide an API for search rather than getting raw container from `moves_`
    const vector<Vec2D>& cells = moves_.moves();
    Vec2D pos = GetCursorCell();

    auto found = std::find(cells.begin(), cells.end(), pos);
    if (found != cells.end()) {
      uint32_t move_id = found - cells.begin();
      LOG_DEBUG("Move to pos (%d, %d) / move_id : %u", pos.x, pos.y, move_id);
      gv_->PushUIState(new StateUIMoving(WrapBase(), unit_key_, pos, core::MoveKey{move_id}));
    } else {
      //      gv_->ChangeStateUI(new StateUIView(WrapBase()));
    }
  } else if (e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

bool StateUIUnitSelected::OnKeyEvent(const foundation::KeyEvent& e) {
  if (e.IsKeyUp()) {
    switch (key_mapper_.Get(e.GetCode())) {
      case KeyCmd::kBack:
        gv_->PopUIState();
        break;
      default:
        break;
    }
  }
  return true;
}

// StateUIMoving

StateUIMoving::StateUIMoving(StateUI::Base base, const core::UnitKey& unit_key, Vec2D dest,
                             const core::MoveKey& move_id)
    : StateUI(base),
      unit_key_(unit_key),
      unit_id_(gi_->QueryUnits().Get(unit_key_)),
      dest_(dest),
      frames_(-1),
      flag_(Flag::kInputActNext),
      move_id_(move_id) {
  path_ = gi_->GetPath(unit_id_, dest_);
  ASSERT(dest_ == path_[0]);
}

// TODO Remove code duplication with above one
StateUIMoving::StateUIMoving(StateUI::Base base, const core::UId& unit_id, Vec2D dest)
    : StateUI(base), unit_id_(unit_id), dest_(dest), frames_(-1), flag_(Flag::kNone), move_id_{0xffffffff} {
  path_ = gi_->GetPath(unit_id_, dest_);
  ASSERT(dest_ == path_[0]);
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

void StateUIMoving::Enter() { gv_->SetSkipRender(unit_id_, true); }

void StateUIMoving::Exit() { gv_->SetSkipRender(unit_id_, false); }

void StateUIMoving::Update() {
  frames_++;
  if (LastFrame()) {  // Arrived at the destination
    if (path_.size() > 1) {
      //      Direction dir = Vec2DRelativePosition(path_[1], path_[0]);
      //      unit_->direction(dir);  // Do this here?
    }
    if (flag_ == Flag::kInputActNext) {
      gv_->ChangeUIState(new StateUIAction(WrapBase(), unit_key_, move_id_));
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

  int frames_current = frames_ % kFramesPerCell;
  int sprite_no = frames_current / (kFramesPerCell / 2);
  float percentage = frames_current / (float)kFramesPerCell;
  Direction dir = Vec2DRelativePosition(path_[path_idx], path_[path_idx - 1]);
  Vec2D diff = path_[path_idx - 1] - path_[path_idx];
  Vec2D diff_pos = diff * (percentage * (float)config::kBlockSize);
  drawer->CopySprite(gv_->GetModelId(unit_id_), kSpriteMove, dir, sprite_no, {kEffectNone, 0}, path_[path_idx],
                     diff_pos);
  gv_->CenterCamera(path_[path_idx] * config::kBlockSize + diff_pos + (config::kBlockSize / 2));
}

// StateUIMagic

StateUIMagic::StateUIMagic(StateUI::Base base, const core::UId& atk_id, const core::UId& def_id,
                           const core::Magic* magic, bool hit, int hp_diff)
    : StateUI(base),
      unit_id_atk_(atk_id),
      unit_id_def_(def_id),
      atk_(gi_->GetUnit(unit_id_atk_)),
      def_(gi_->GetUnit(unit_id_def_)),
      magic_(magic),
      hit_(hit),
      hp_diff_(hp_diff),
      animator_(nullptr) {}

StateUIMagic::~StateUIMagic() {
  ASSERT(animator_ != NULL);
  delete animator_;
}

void StateUIMagic::Enter() {
  gv_->SetSkipRender(unit_id_atk_, true);
  gv_->SetSkipRender(unit_id_def_, true);
}

void StateUIMagic::Exit() {
  gv_->SetSkipRender(unit_id_atk_, false);
  gv_->SetSkipRender(unit_id_def_, false);
}

void StateUIMagic::Render(Drawer* drawer) {
  if (animator_ == NULL) {
    TextureManager* tm = drawer->GetTextureManager();
    Texture* texture = tm->FetchTexture(rcpath::MagicPath(magic_->GetId()).ToString());
    texture->SetAlpha(160);  // FIXME non-fixed alpha value
    animator_ = new TextureAnimator(texture, kFramesPerCut);
  }

  Vec2D unit_pos = atk_->position();
  Vec2D def_pos = def_->position();
  Direction dir = Vec2DRelativePosition(unit_pos, def_pos);

  drawer->CopySprite(gv_->GetModelId(unit_id_atk_), kSpriteAttack, dir, 0, {kEffectNone, 0}, unit_pos);

  const SpriteType target_sprite_hit = magic_->is_target_enemy() ? kSpriteDamaged : kSpriteBuff;
  const SpriteType target_sprite = hit_ ? target_sprite_hit : kSpriteBlocked;

  drawer->CopySprite(gv_->GetModelId(unit_id_def_), target_sprite, OppositeDirection(dir), 0, {kEffectNone, 0},
                     def_pos);

  Rect src_rect = animator_->GetCurrentCutRect();
  drawer->CopyTextureToCell(animator_->GetTexture(), &src_rect, def_pos);
}

void StateUIMagic::Update() {
  if (animator_) {
    animator_->NextFrame();
    if (animator_->DoneAnimate()) {
      if (hit_) {
        gv_->ChangeUIState(new StateUIUnitTooltipAnim(WrapBase(), unit_id_def_, hp_diff_, 0));
      } else {
        gv_->PopUIState();
      }
    }
  }
}

// StateUIKilled

StateUIKilled::StateUIKilled(StateUI::Base base, const core::UId& unit_id)
    : StateUI(base), unit_id_(unit_id), frames_(0) {}

void StateUIKilled::Enter() {
  gv_->SetSkipRender(unit_id_, true);
  Misc::SetShowCursor(false);
}

void StateUIKilled::Exit() {
  gv_->SetSkipRender(unit_id_, false);
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
  const core::Unit* unit = gi_->GetUnit(unit_id_);
  drawer->CopySprite(gv_->GetModelId(unit_id_), kSpriteLowHP, kDirNone, 0, {kEffectShade, progress}, unit->position());
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
  const core::Map* map = gi_->GetMap();
  std::string name = map->GetTerrain(coords_)->GetName();
  gv_->terrain_info_view()->SetText(name);
  gv_->terrain_info_view()->visible(true);
}

void StateUIEmptySelected::Exit() { gv_->terrain_info_view()->visible(false); }

void StateUIEmptySelected::Render(Drawer* drawer) {
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(coords_, 4);
}

bool StateUIEmptySelected::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  if (e.IsLeftButtonUp() || e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

// StateUIAttack

StateUIAttack::StateUIAttack(StateUI::Base base, const core::UId& atk_id, const core::UId& def_id, bool hit,
                             bool critical, int damage)
    : StateUI(base),
      unit_id_atk_(atk_id),
      unit_id_def_(def_id),
      atk_(gi_->GetUnit(unit_id_atk_)),
      def_(gi_->GetUnit(unit_id_def_)),
      hit_(hit),
      critical_(critical),
      damage_(damage),
      frames_(-1) {}

void StateUIAttack::Enter() {
  gv_->SetSkipRender(unit_id_atk_, true);
  gv_->SetSkipRender(unit_id_def_, true);
  Misc::SetShowCursor(false);
}

void StateUIAttack::Exit() {
  gv_->SetSkipRender(unit_id_atk_, false);
  gv_->SetSkipRender(unit_id_def_, false);
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
  const CutInfo* cut_atk = &kCutInfoAtk[cut_no];
  const CutInfo* cut_def = hit_ ? &kCutInfoDefDamaged[cut_no] : &kCutInfoDefBlocked[cut_no];
  Vec2D atk_pos = atk_->position();
  Vec2D def_pos = def_->position();
  Vec2D atk_offset = GenerateVec2DOffset(atk_->direction(), cut_atk->offset);
  Vec2D def_offset = GenerateVec2DOffset(def_->direction(), cut_def->offset);
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

  if (atk_->position().y > def_->position().y) {
    drawer->CopySprite(gv_->GetModelId(unit_id_def_), cut_def->sprite, OppositeDirection(dir), cut_def->no,
                       {kEffectNone, 0}, def_pos, def_offset);
    drawer->CopySprite(gv_->GetModelId(unit_id_atk_), cut_atk->sprite, dir, cut_atk->no, sprite_effect, atk_pos,
                       atk_offset);
  } else {
    drawer->CopySprite(gv_->GetModelId(unit_id_atk_), cut_atk->sprite, dir, cut_atk->no, sprite_effect, atk_pos,
                       atk_offset);
    drawer->CopySprite(gv_->GetModelId(unit_id_def_), cut_def->sprite, OppositeDirection(dir), cut_def->no,
                       {kEffectNone, 0}, def_pos, def_offset);
  }

  // Render Damage Text
  if (frames_ >= 5 * kFramesPerCut) {
    const int text_size = 13;
    Vec2D damage_text_pos = def_pos * config::kBlockSize;
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
      gv_->ChangeUIState(new StateUIUnitTooltipAnim(WrapBase(), unit_id_def_, -damage_, 0));
    } else {
      gv_->PopUIState();
    }
  }
}

// StateUIUnitTooltipAnim

StateUIUnitTooltipAnim::StateUIUnitTooltipAnim(StateUI::Base base, const core::UId& unit_id, int hp, int mp)
    : StateUI(base), frames_(-1), unit_id_(unit_id), hp_(hp), mp_(mp) {
  auto unit = gi_->GetUnit(unit_id_);
  const core::HpMp& cur_hpmp = unit->GetCurrentHpMp();
  const core::HpMp& max_hpmp = unit->GetOriginalHpMp();
  int cur_hp = cur_hpmp.hp;
  int cur_mp = cur_hpmp.mp;
  int max_hp = max_hpmp.hp;
  int max_mp = max_hpmp.mp;

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

  auto unit = gi_->GetUnit(unit_id_);
  gv_->unit_tooltip_view()->SetCoordsByUnitCoords(unit->position(), gv_->GetCameraCoords(), gv_->GetFrameSize());
}

void StateUIUnitTooltipAnim::Exit() {
  Misc::SetShowCursor(true);
  gv_->unit_tooltip_view()->visible(false);
}

void StateUIUnitTooltipAnim::Update() {
  auto unit = gi_->GetUnit(unit_id_);

  frames_++;
  if (LastFrame()) {
    gv_->PopUIState();
  }

  const int max_anim_frames = (kFrames - 1) * 1 / 2;
  const int cur_anim_frames = std::min(max_anim_frames, frames_);

  core::HpMp hpmp_mod = unit->GetCurrentHpMp();

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

  // TODO Update only HP and MP
  gv_->unit_tooltip_view()->SetContents(unit->id(), unit->GetLevel(), hpmp_mod, unit->GetOriginalHpMp(), hpmp_rem);
}

void StateUIUnitTooltipAnim::Render(Drawer*) {}

// StateUITargeting

StateUITargeting::StateUITargeting(StateUI::Base base, const core::UnitKey& unit_key, const core::MoveKey& move_key,
                                   const string& magic_id)
    : StateUIOperable(base),
      unit_key_(unit_key),
      unit_id_(gi_->QueryUnits().Get(unit_key_)),
      move_key_(move_key),
      pos_(gi_->QueryMoves(unit_key_).Get(move_key_)),
      magic_id_(magic_id),
      is_basic_attack_(!magic_id.compare("basic_attack")),
      range_(GetRange()),
      acts_(gi_->QueryActs(unit_key_, move_key_,
                           is_basic_attack_ ? core::ActionType::kBasicAttack : core::ActionType::kMagic)) {
  key_mapper_.Set(SDLK_ESCAPE, KeyCmd::kBack);

  Rect frame = LayoutHelper::CalcPosition(gv_->GetFrameSize(), {200, 100}, LayoutHelper::kAlignLftBot,
                                          LayoutHelper::kDefaultSpace);
  frame.Move(frame.w() + LayoutHelper::kDefaultSpace, 0);
}

const core::AttackRange& StateUITargeting::GetRange() {
  if (is_basic_attack_) {
    return gi_->GetUnit(unit_id_)->attack_range();
  } else {
    const core::Magic* magic = gi_->GetMagic(magic_id_);
    return magic->GetRange();
  }
}

void StateUITargeting::Enter() {
  StateUIOperable::Enter();
  gv_->SetSkipRender(unit_id_, true);
}

void StateUITargeting::Exit() {
  gv_->unit_tooltip_view()->visible(false);
  gv_->SetSkipRender(unit_id_, false);
  StateUIOperable::Exit();
}

void StateUITargeting::Render(Drawer* drawer) {
  gv_->RenderUnit(drawer, gi_->GetUnit(unit_id_), pos_);

  // Show Attack Range
  range_.ForEach(
      [&](Vec2D d) {
        if (!gi_->IsValidCoords(d)) return;
        drawer->SetDrawColor(Color(255, 64, 64, 128));
        drawer->FillCell(d);
      },
      pos_);

  StateUIOperable::Render(drawer);
}

void StateUITargeting::Update() { StateUIOperable::Update(); }

bool StateUITargeting::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  if (e.IsLeftButtonUp()) {
    Vec2D map_pos = GetCursorCell();

    if (is_basic_attack_) {
      auto act_id = acts_.Find(map_pos);
      if (act_id) {
        gv_->InitUIStateMachine();
        gi_->PushAction(unit_key_, move_key_, core::ActionType::kBasicAttack, act_id);
        LOG_DEBUG("Pushing Action {UnitKey:%u, MoveKey:%u, BasicAttack ActId:%u}", unit_key_.Value(), move_key_.Value(),
                  act_id.Value());
      }
    } else {
      auto act_id = acts_.FindMagic(magic_id_, map_pos);
      if (act_id) {
        gv_->InitUIStateMachine();
        gi_->PushAction(unit_key_, move_key_, core::ActionType::kMagic, act_id);
        LOG_DEBUG("Pushing Action {UnitKey:%u, MoveKey:%u, BasicAttack ActId:%u}", unit_key_.Value(), move_key_.Value(),
                  act_id.Value());
      }
    }
  } else if (e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

bool StateUITargeting::OnKeyEvent(const foundation::KeyEvent& e) {
  if (e.IsKeyUp()) {
    switch (key_mapper_.Get(e.GetCode())) {
      case KeyCmd::kBack:
        gv_->PopUIState();
        break;
      default:
        break;
    }
  }
  return true;
}

bool StateUITargeting::OnMouseMotionEvent(const foundation::MouseMotionEvent& e) {
  StateUIOperable::OnMouseMotionEvent(e);

  auto unit_tooltip_view = gv_->unit_tooltip_view();
  const core::Map* map = gi_->GetMap();
  Vec2D cursor_cell = GetCursorCell();
  const core::Unit* unit_target = gi_->GetUnit(cursor_cell);
  const core::Unit* unit = gi_->GetUnit(unit_key_);

  if (unit_target) {
    bool hostile = unit->IsHostile(unit_target);
    int hp_diff = 0;
    int accuracy = 0;
    if ((is_basic_attack_ && hostile) ||
        (!is_basic_attack_ && (gi_->GetMagic(magic_id_)->is_target_enemy() == hostile))) {
      if (is_basic_attack_) {
        hp_diff = -core::Formulae::ComputeBasicAttackDamage(map, unit, unit_target);
        accuracy = core::Formulae::ComputeBasicAttackAccuracy(unit, unit_target);
      } else {
        const auto magic = gi_->GetMagic(magic_id_);
        hp_diff = magic->HPDiff(unit, unit_target);
        accuracy = magic->CalcAccuracy(unit, unit_target);
      }

      unit_tooltip_view->SetUnitActionInfo(unit_target, accuracy, hp_diff);
    } else {
      unit_tooltip_view->SetUnitTerrainInfo(map->GetCell(cursor_cell), unit_target);
    }
    unit_tooltip_view->SetCoordsByUnitCoords(unit_target->position(), gv_->GetCameraCoords(), gv_->GetFrameSize());
  }
  unit_tooltip_view->visible(unit_target != nullptr);
  return true;
}

// StateUIAction

StateUIAction::StateUIAction(StateUI::Base base, const core::UnitKey& unit_key, const core::MoveKey& move_id)
    : StateUI(base), unit_key_(unit_key), unit_id_(gi_->QueryUnits().Get(unit_key)), move_id_(move_id) {
  key_mapper_.Set(SDLK_ESCAPE, KeyCmd::kBack);

  pos_ = gi_->QueryMoves(unit_key_).Get(move_id_);
}

void StateUIAction::Enter() {
  gv_->SetSkipRender(unit_id_, true);
  UnitActionView* unit_action_view = gv_->unit_action_view();
  unit_action_view->SetUnitAndMoveKey(unit_key_, move_id_);
  unit_action_view->SetCoords(layout::CalcPositionNearUnit(unit_action_view->GetFrameSize(), gv_->GetFrameSize(),
                                                           gv_->GetCameraCoords(), pos_));
  unit_action_view->visible(true);
}

void StateUIAction::Exit() {
  gv_->SetSkipRender(unit_id_, false);
  UnitActionView* unit_action_view = gv_->unit_action_view();
  unit_action_view->visible(false);
}

void StateUIAction::Render(Drawer* drawer) {
  gv_->RenderUnit(drawer, gi_->GetUnit(unit_id_), pos_);
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(pos_, 4);
}

bool StateUIAction::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  if (e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

bool StateUIAction::OnKeyEvent(const foundation::KeyEvent& e) {
  if (e.IsKeyUp()) {
    switch (key_mapper_.Get(e.GetCode())) {
      case KeyCmd::kBack:
        gv_->PopUIState();
        break;
      default:
        break;
    }
  }
  return true;
}

// StateUIMagicSelection

StateUIMagicSelection::StateUIMagicSelection(StateUI::Base base, const core::UnitKey& unit_key,
                                             const core::MoveKey& move_id)
    : StateUI(base), unit_key_(unit_key), unit_id_(gi_->QueryUnits().Get(unit_key_)), move_id_(move_id) {
  key_mapper_.Set(SDLK_ESCAPE, KeyCmd::kBack);

  pos_ = gi_->QueryMoves(unit_key).Get(move_id_);
}

void StateUIMagicSelection::Enter() {
  auto magic_list = gi_->GetMagicList(unit_id_);
  MagicListView* mlv = gv_->magic_list_view();
  mlv->SetData(unit_key_, move_id_, magic_list);
  mlv->SetCoords(layout::CalcPositionNearUnit(mlv->GetFrameSize(), gv_->GetFrameSize(), gv_->GetCameraCoords(), pos_));
  mlv->visible(true);
  gv_->SetSkipRender(unit_id_, true);
}

void StateUIMagicSelection::Exit() {
  gv_->magic_list_view()->visible(false);
  gv_->SetSkipRender(unit_id_, false);
}

void StateUIMagicSelection::Render(Drawer* drawer) {
  gv_->RenderUnit(drawer, gi_->GetUnit(unit_key_), pos_);
  drawer->SetDrawColor(Color(0, 255, 0, 128));
  drawer->BorderCell(pos_, 4);
}

bool StateUIMagicSelection::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  if (e.IsRightButtonUp()) {
    gv_->PopUIState();
  }
  return true;
}

bool StateUIMagicSelection::OnKeyEvent(const foundation::KeyEvent& e) {
  if (e.IsKeyUp()) {
    switch (key_mapper_.Get(e.GetCode())) {
      case KeyCmd::kBack:
        gv_->PopUIState();
        break;
      default:
        break;
    }
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

  gv_->NextFrame([gv = gv_, gi = gi_]() {
    ControlView* control_view = gv->control_view();
    const auto& turn = gi->GetTurn();
    control_view->SetTurnText(turn.current(), turn.limit());
    control_view->SetEndTurnVisible(gi->IsUserTurn());
  });
}

// StateUISpeak

StateUISpeak::StateUISpeak(StateUI::Base base, const core::UId& unit_id, const string& words)
    : StateUI(base), unit_id_(unit_id), words_(words) {}

void StateUISpeak::Enter() {
  auto unit = gi_->GetUnit(unit_id_);
  auto unit_dialog_view = gv_->unit_dialog_view();
  auto unit_dialog_view_wrapper = gv_->unit_dialog_view_wrapper();
  unit_dialog_view->SetUnit(unit);
  unit_dialog_view->SetText(words_);
  unit_dialog_view->SetCoords(layout::CalcPositionNearUnit(gv_->unit_dialog_view()->GetFrameSize(), gv_->GetFrameSize(),
                                                           gv_->GetCameraCoords(), unit->position()));
  unit_dialog_view_wrapper->visible(true);
}

void StateUISpeak::Exit() { gv_->unit_dialog_view_wrapper()->visible(false); }

void StateUISpeak::Update() {
  if (!gv_->unit_dialog_view_wrapper()->visible()) {
    gv_->PopUIState();
  }
}

bool StateUISpeak::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
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
    gv_->EndStage();
  }
}

//
// StateUIPromote
//

StateUIPromote::StateUIPromote(StateUI::Base base, const core::UId& uid) : StateUI{base}, uid_{uid} {}

void StateUIPromote::Enter() {
  ModalDialogView* modal_dialog_view = gv_->dialog_view();

  const auto unit = gi_->GetUnit(uid_);
  const auto class_id = unit->unit_class()->promotion_info()->id;
  modal_dialog_view->SetText(boost::str(boost::format("%s has been promoted to %s") % unit->id() % class_id));
  modal_dialog_view->visible(true);
}

void StateUIPromote::Update() {
  ModalDialogView* modal_dialog_view = gv_->dialog_view();

  gv_->UpdateModelId(uid_);  // FIXME DO NOT do this every frame

  if (!modal_dialog_view->visible()) {
    gv_->PopUIState();
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

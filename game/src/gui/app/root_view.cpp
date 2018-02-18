#include "root_view.h"

#include "app.h"
#include "config.h"
#include "control_view.h"
#include "core/assets.h"
#include "core/cell.h"
#include "core/game.h"
#include "core/magic_list.h"
#include "core/scenario.h"
#include "deploy_view.h"
#include "equipment_select_view.h"
#include "gui/foundation/texture_manager.h"
#include "gui/uifw/composite_view.h"
#include "gui/uifw/drawer.h"
#include "gui/uifw/layout_helper.h"
#include "gui/uifw/modal_dialog_view.h"
#include "gui/uifw/modal_view.h"
#include "gui/uifw/scroll_view.h"
#include "layout_helper.h"
#include "magic_list_view.h"
#include "state_ui.h"
#include "terrain_info_view.h"
#include "unit_action_view.h"
#include "unit_dialog_view.h"
#include "unit_info_view.h"
#include "unit_list_view.h"
#include "unit_view.h"
#include "util/state_machine.h"

namespace mengde {
namespace gui {
namespace app {

RootView::RootView(const Vec2D size, core::Scenario* scenario, App* app)
    : View(),
      game_(scenario->GetGame()),
      app_(app),
      ui_views_(nullptr),
      deploy_view_(nullptr),
      unit_info_view_(nullptr),
      unit_view_(nullptr),
      control_view_(nullptr),
      dialog_view_(nullptr),
      magic_list_view_(nullptr),
      terrain_info_view_(nullptr),
      unit_list_view_(nullptr),
      unit_action_view_(nullptr),
      ui_state_machine_(new StateUIView({game_, this})),
      reserved_callbacks_(),
      mouse_coords_(0, 0),
      camera_coords_(0, 0),
      max_camera_coords_(size),
      frame_count_(-1) {
  const Rect frame = {0, 0, size.x, size.y};
  SetFrame(&frame);

  ui_views_ = new CompositeView(frame);
  ui_views_->SetTransparent();

  {  // Initialize deploy_view_
    core::Assets* assets = scenario->GetAssets();
    Rect          frame  = LayoutHelper::CalcPosition(GetFrameSize(), {680, 480}, LayoutHelper::kAlignCenter);
    deploy_view_         = new DeployView(frame, assets, game_);
    deploy_view_->visible(true);
    ui_views_->AddChild(deploy_view_);
  }

  // Calculate max_camera_coords_
  const int kMapWidth     = game_->GetMapSize().x * config::kBlockSize;
  const int kMapHeight    = game_->GetMapSize().y * config::kBlockSize;
  const int kWindowWidth  = GetFrame()->GetW();
  const int kWindowHeight = GetFrame()->GetH();
  const int kMaxCameraX   = kMapWidth - kWindowWidth;
  const int kMaxCameraY   = kMapHeight - kWindowHeight;
  max_camera_coords_      = {kMaxCameraX, kMaxCameraY};

  // Create UI views

  Rect unit_frame =
      LayoutHelper::CalcPosition(GetFrameSize(), {220, 320}, LayoutHelper::kAlignRgtTop, LayoutHelper::kDefaultSpace);
  unit_view_ = new UnitView(unit_frame);
  //  unit_view_->visible(false);
  ui_views_->AddChild(unit_view_);

  {  // Initialize control_view_
    Rect control_frame =
        LayoutHelper::CalcPosition(GetFrameSize(), {300, 136}, LayoutHelper::kAlignRgtBot, LayoutHelper::kDefaultSpace);
    control_view_ = new ControlView(&control_frame, game_, this);
    ui_views_->AddChild(control_view_);
  }

  {  // Initialize dialog_view_
    Rect dialog_frame = *GetFrame();
    dialog_view_      = new ModalDialogView(&dialog_frame, "Put your message here!");
    dialog_view_->visible(false);
    ui_views_->AddChild(dialog_view_);
  }

  {  // Initialize magic_list_view_;
    Rect magic_list_frame = {8, 8, 170, 200};
    magic_list_view_      = new MagicListView(magic_list_frame, game_, this);
    magic_list_view_->visible(false);
    ui_views_->AddChild(magic_list_view_);
  }

  {  // Initalize unit_info_view_
    Rect unit_info_frame =
        LayoutHelper::CalcPosition(GetFrameSize(), {200, 100}, LayoutHelper::kAlignLftBot, LayoutHelper::kDefaultSpace);
    unit_info_view_ = new UnitInfoView(&unit_info_frame);
    ui_views_->AddChild(unit_info_view_);
  }

  {  // Initialize unit_dialog_view_
    Rect unit_dialog_frame =
        LayoutHelper::CalcPosition(GetFrameSize(), {360, 120}, LayoutHelper::kAlignCenter, LayoutHelper::kDefaultSpace);
    unit_dialog_view_ = new UnitDialogView(&unit_dialog_frame);
    unit_dialog_view_wrapper_ =
        new ModalView(GetActualFrame(), unit_dialog_view_, [=](const foundation::MouseButtonEvent e) {
          if (e.IsLeftButtonUp() || e.IsRightButtonUp()) {
            unit_dialog_view_wrapper_->visible(false);
          }
          return true;
        });
    unit_dialog_view_wrapper_->visible(false);
    ui_views_->AddChild(unit_dialog_view_wrapper_);
  }

  {  // Initialize terrain_info_view_
    Rect terrain_frame =
        LayoutHelper::CalcPosition(GetFrameSize(), {200, 100}, LayoutHelper::kAlignLftBot, LayoutHelper::kDefaultSpace);
    terrain_info_view_ = new TerrainInfoView(terrain_frame, {0, 0});  // FIXME temporary coords
    terrain_info_view_->visible(false);
    ui_views_->AddChild(terrain_info_view_);
  }

  {  // Initialize unit_list_view_
    Rect frame      = LayoutHelper::CalcPosition(GetFrameSize(), {680, 480}, LayoutHelper::kAlignCenter);
    unit_list_view_ = new UnitListView(frame, game_->GetCurrentTurnUnits());
    unit_list_view_->visible(false);
    ui_views_->AddChild(unit_list_view_);
  }

  {  // Initialize unit_action_view_
    Rect frame        = LayoutHelper::CalcPosition(GetFrameSize(), {150, 150}, LayoutHelper::kAlignLftTop);
    unit_action_view_ = new UnitActionView(frame, game_, this);
    unit_action_view_->visible(false);
    ui_views_->AddChild(unit_action_view_);
  }
}

RootView::~RootView() {
  // FIXME ui_view_ is deleted first then StateMachine destructs and it could do something with ui_views_ then
  //       we are doomed.
  delete ui_views_;
}

void RootView::Update() {
  frame_count_++;
  RunCallbacks();
  GetCurrentState()->Update();
  control_view_->Update();
}

void RootView::Render(Drawer* drawer) {
  drawer->SetOffset(camera_coords_);

  // Render Background
  const int       kScreenWidth  = drawer->GetWindowSize().x;
  const int       kScreenHeight = drawer->GetWindowSize().y;
  string          path          = game_->GetMapBitmapPath();
  TextureManager* tm            = drawer->GetTextureManager();
  Texture*        background    = tm->FetchTexture(path);
  Rect            src_rect      = {0, 0, kScreenWidth, kScreenHeight};
  Rect            dst_rect      = {0, 0, kScreenWidth, kScreenHeight};
  drawer->CopyTextureBackground(background, &src_rect, &dst_rect);

  // Render by UI State
  GetCurrentState()->Render(drawer);

  // Render units
  game_->ForEachUnit([this, drawer](core::Unit* unit) {
    if (unit->IsNoRender() || unit->IsDead()) return;

    SpriteType   stype         = unit->IsHPLow() ? kSpriteLowHP : kSpriteMove;
    int          sprite_no     = this->GetCurrentSpriteNo(2, app_->GetMaxFps() / 2);
    SpriteEffect sprite_effect = {kEffectNone, 0};
    if (unit->IsDoneAction()) {
      stype         = kSpriteStand;
      sprite_no     = 0;
      sprite_effect = {kEffectShade, 128};
    }
    drawer->CopySprite(unit->GetModelId(), stype, unit->GetDirection(), sprite_no, sprite_effect, unit->GetPosition());
  });

  drawer->SetOffset({0, 0});

  ui_views_->Render(drawer);
}

bool RootView::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (ui_views_->OnMouseButtonEvent(e)) return true;
  return GetCurrentState()->OnMouseButtonEvent(e);
}

bool RootView::OnMouseMotionEvent(const foundation::MouseMotionEvent e) {
  mouse_coords_ = e.GetCoords();

  if (ui_views_->OnMouseMotionEvent(e)) return true;
  if (GetCurrentState()->OnMouseMotionEvent(e)) return true;

  return true;
}

bool RootView::OnMouseWheelEvent(const foundation::MouseWheelEvent e) {
  if (ui_views_->DelegateMouseWheelEvent(e)) return true;
  if (GetCurrentState()->OnMouseWheelEvent(e)) return true;

  return true;
}

void RootView::MoveCameraX(int d) {
  camera_coords_.x += d;
  if (camera_coords_.x < 0) camera_coords_.x = 0;
  if (camera_coords_.x > max_camera_coords_.x) camera_coords_.x = max_camera_coords_.x;
}

void RootView::MoveCameraY(int d) {
  camera_coords_.y += d;
  if (camera_coords_.y < 0) camera_coords_.y = 0;
  if (camera_coords_.y > max_camera_coords_.y) camera_coords_.y = max_camera_coords_.y;
}

void RootView::CenterCamera(Vec2D coords) {
  coords -= (GetFrameSize() / 2);
  camera_coords_ = coords.Bound({0, 0}, max_camera_coords_);
}

void RootView::RaiseMouseMotionEvent() {
  Vec2D mouse_pos;
  SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);  // FIXME Not to use SDL interface directly
  OnMouseMotionEvent(foundation::MouseMotionEvent(MouseMotionEvent::Type::kOver, mouse_pos, mouse_pos));
}

void RootView::NextFrame(NextFrameCallback cb) { reserved_callbacks_.push(cb); }

void RootView::RunCallbacks() {
  while (!reserved_callbacks_.empty()) {
    reserved_callbacks_.front()();
    reserved_callbacks_.pop();
  }
}

void RootView::ChangeUIState(StateUI* state_ui) {
  NextFrame([this, state_ui]() {
    this->ui_state_machine_.ChangeState(state_ui);
    this->RaiseMouseMotionEvent();
  });
}

void RootView::PushUIState(StateUI* state_ui) {
  NextFrame([this, state_ui]() {
    this->ui_state_machine_.PushState(state_ui);
    this->RaiseMouseMotionEvent();
  });
}

void RootView::PopUIState() {
  NextFrame([this]() {
    this->ui_state_machine_.PopState();
    this->RaiseMouseMotionEvent();
  });
}

void RootView::InitUIStateMachine() {
  NextFrame([this]() {
    ui_state_machine_.InitState();
    if (game_->HasNext()) {
      ui_state_machine_.PushState(new StateUIDoCmd({game_, this}));
    }
  });
}

int RootView::GetCurrentSpriteNo(int num_sprites, int frames_per_sprite) const {
  return (frame_count_ / frames_per_sprite) % num_sprites;
}

void RootView::EndGame() {
  NextFrame([=]() { app_->EndGame(); });
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

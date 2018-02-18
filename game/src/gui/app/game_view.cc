#include "game_view.h"

#include "config.h"
#include "core/assets.h"
#include "core/game.h"
#include "core/unit.h"
#include "gui/foundation/texture_manager.h"
#include "gui/uifw/sprite_type.h"
#include "ui_views.h"
#include "app.h"
#include "gui/uifw/drawer.h"

namespace mengde {
namespace gui {
namespace app {

GameView::GameView(const Rect& frame, core::Game* game, App* app)
    : View(frame),
      game_(game),
      app_(app),
      ui_state_machine_(new StateUIView({game_, this})),
      frame_callbacks_(),
      mouse_coords_(0, 0),
      camera_coords_(0, 0),
      max_camera_coords_(0, 0),
      frame_count_(-1),
      ui_views_(nullptr) {
  // Calculate max_camera_coords_
  const int kMapWidth     = game_->GetMapSize().x * config::kBlockSize;
  const int kMapHeight    = game_->GetMapSize().y * config::kBlockSize;
  const int kWindowWidth  = GetFrame()->GetW();
  const int kWindowHeight = GetFrame()->GetH();
  const int kMaxCameraX   = kMapWidth - kWindowWidth;
  const int kMaxCameraY   = kMapHeight - kWindowHeight;
  max_camera_coords_      = {kMaxCameraX, kMaxCameraY};
}

void GameView::Update() {
  frame_count_++;
  RunCallbacks();
  GetCurrentState()->Update();
}

void GameView::Render(Drawer* drawer) {
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
}

bool GameView::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  return GetCurrentState()->OnMouseButtonEvent(e);
}

bool GameView::OnMouseMotionEvent(const foundation::MouseMotionEvent e) {
  mouse_coords_ = e.GetCoords();
  return GetCurrentState()->OnMouseMotionEvent(e);
}

bool GameView::OnMouseWheelEvent(const foundation::MouseWheelEvent e) {
  return GetCurrentState()->OnMouseWheelEvent(e);
}

void GameView::ChangeUIState(StateUI* state_ui) {
  NextFrame([this, state_ui]() {
    this->ui_state_machine_.ChangeState(state_ui);
    this->RaiseMouseOverEvent();
  });
}

void GameView::PushUIState(StateUI* state_ui) {
  NextFrame([this, state_ui]() {
    this->ui_state_machine_.PushState(state_ui);
    this->RaiseMouseOverEvent();
  });
}

void GameView::PopUIState() {
  NextFrame([this]() {
    this->ui_state_machine_.PopState();
    this->RaiseMouseOverEvent();
  });
}

void GameView::InitUIStateMachine() {
  NextFrame([this]() {
    ui_state_machine_.InitState();
    if (game_->HasNext()) {
      ui_state_machine_.PushState(new StateUIDoCmd({game_, this}));
    }
  });
}

void GameView::MoveCameraX(int d) {
  camera_coords_.x += d;
  if (camera_coords_.x < 0) camera_coords_.x = 0;
  if (camera_coords_.x > max_camera_coords_.x) camera_coords_.x = max_camera_coords_.x;
}

void GameView::MoveCameraY(int d) {
  camera_coords_.y += d;
  if (camera_coords_.y < 0) camera_coords_.y = 0;
  if (camera_coords_.y > max_camera_coords_.y) camera_coords_.y = max_camera_coords_.y;
}

void GameView::CenterCamera(Vec2D center) {
  Vec2D coords = center - (GetFrameSize() / 2);
  camera_coords_ = coords.Bound({0, 0}, max_camera_coords_);
}

void GameView::EndGame() {
  NextFrame([=]() { app_->EndGame(); });
}

// private methods

void GameView::NextFrame(NextFrameCallback cb) { frame_callbacks_.push(cb); }

void GameView::RaiseMouseOverEvent() {
  Vec2D mouse_pos;
  SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);  // FIXME Not to use SDL interface directly
  OnMouseMotionEvent(foundation::MouseMotionEvent(MouseMotionEvent::Type::kOver, mouse_pos, mouse_pos));
}

void GameView::RunCallbacks() {
  while (!frame_callbacks_.empty()) {
    frame_callbacks_.front()();
    frame_callbacks_.pop();
  }
}

int GameView::GetCurrentSpriteNo(int num_sprites, int frames_per_sprite) const {
  return (frame_count_ / frames_per_sprite) % num_sprites;
}

}  // namespace app
}  // namespace gui
}  // namespace mengde


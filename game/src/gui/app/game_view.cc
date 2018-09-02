#include "game_view.h"

#include "app.h"
#include "config.h"
#include "core/assets.h"
#include "core/stage.h"
#include "core/unit.h"
#include "core/user_interface.h"
#include "gui/foundation/texture_manager.h"
#include "gui/uifw/drawer.h"
#include "gui/uifw/sprite_type.h"
#include "model_finder.h"
#include "resource_path.h"
#include "ui_views.h"

namespace mengde {
namespace gui {
namespace app {

GameView::GameView(const Rect& frame, core::UserInterface* gi, App* app)
    : View(frame),
      gi_{gi},
      app_(app),
      ui_state_machine_(),
      frame_callbacks_(),
      mouse_coords_(0, 0),
      camera_coords_(0, 0),
      max_camera_coords_(0, 0),
      frame_count_(-1),
      ui_views_(nullptr) {
  // Calculate max_camera_coords_
  const Vec2D kMapSize = gi_->GetMapSize() * config::kBlockSize;
  const Vec2D kWindowSize = GetFrameSize();
  max_camera_coords_ = kMapSize - kWindowSize;

  ui_state_machine_.InitState();
  ui_state_machine_.PushState(new StateUIView({gi_, this}));
}

GameView::~GameView() {}

void GameView::Update() {
  frame_count_++;
  RunCallbacks();
  GetCurrentState()->Update();
}

void GameView::Render(Drawer* drawer) {
  drawer->SetOffset(camera_coords_);

  // Render Background
  Path path = rcpath::MapPath(gi_->GetMapId());
  TextureManager* tm = drawer->GetTextureManager();
  Texture* background = tm->FetchTexture(path.ToString());

  const Vec2D kScreenSize = drawer->GetWindowSize();
  Rect src_rect({0, 0}, kScreenSize);
  Rect dst_rect({0, 0}, kScreenSize);

  drawer->CopyTextureBackground(background, &src_rect, &dst_rect);

  // Render by UI State
  GetCurrentState()->Render(drawer);

  // Render units
  gi_->ForEachUnit([this, drawer](const core::Unit* unit) {
    if (this->SkipRender(unit->uid()) || unit->IsDead()) return;
    RenderUnit(drawer, unit, unit->position());
  });

  drawer->SetOffset({0, 0});
}

bool GameView::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  return GetCurrentState()->OnMouseButtonEvent(e);
}

bool GameView::OnMouseMotionEvent(const foundation::MouseMotionEvent& e) {
  mouse_coords_ = e.GetCoords();
  return GetCurrentState()->OnMouseMotionEvent(e);
}

bool GameView::OnMouseWheelEvent(const foundation::MouseWheelEvent& e) {
  return GetCurrentState()->OnMouseWheelEvent(e);
}

bool GameView::OnKeyEvent(const foundation::KeyEvent& e) { return GetCurrentState()->OnKeyEvent(e); }

void GameView::RenderUnit(Drawer* drawer, const core::Unit* unit, Vec2D pos) {
  SpriteType stype = unit->IsHPLow() ? kSpriteLowHP : kSpriteMove;
  int sprite_no = this->GetCurrentSpriteNo(2, app_->GetMaxFps() / 2);
  SpriteEffect sprite_effect = {kEffectNone, 0};
  if (unit->IsDoneAction()) {
    stype = kSpriteStand;
    sprite_no = 0;
    sprite_effect = {kEffectShade, 128};
  }
  drawer->CopySprite(GetModelId(unit->uid()), stype, unit->direction(), sprite_no, sprite_effect, pos);
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
    ui_state_machine_.PushState(new StateUIView({gi_, this}));
    if (gi_->HasNextCmd()) {
      ui_state_machine_.PushState(new StateUIDoCmd({gi_, this}));
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

void GameView::EndStage() {
  app_->NextFrame([=]() { app_->EndStage(); });
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

void GameView::SetSkipRender(const core::UId& id, bool b) {
  auto o = skip_render_.find(id.Value());
  bool found = (o != skip_render_.end());
  ASSERT(found != b);
  if (found) {
    skip_render_.erase(o);
  } else {
    skip_render_.insert(id.Value());
  }
}

bool GameView::SkipRender(const core::UId& id) const { return skip_render_.find(id.Value()) != skip_render_.end(); }

#include "util/game_env.h"

string GameView::GetModelId(const core::UId& uid) {
  auto found = model_ids_.find(uid.Value());
  if (found == model_ids_.end()) {
    auto unit = gi_->GetUnit(uid);
    auto hero_id = unit->id();
    auto hero_class = unit->unit_class()->GetId();
    auto force = unit->force();
    auto model_id = FindModelId(app_->GetCurrentScenarioPath(), hero_class, hero_id, force);

    model_ids_[uid.Value()] = model_id;
  }

  return model_ids_[uid.Value()];
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

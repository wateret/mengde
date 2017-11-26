#include "root_view.h"
#include "app.h"
#include "game.h"
#include "state_ui.h"
#include "layout_helper.h"
#include "drawer.h"
#include "texture_manager.h"
#include "util/state_machine.h"
#include "ui/unit_info_view.h"
#include "ui/unit_view.h"
#include "ui/unit_dialog_view.h"
#include "ui/control_view.h"
#include "ui/modal_dialog_view.h"
#include "ui/magic_list_view.h"
#include "core/magic_list.h"
#include "core/cell.h"

RootView::RootView(const Vec2D size, Game* game, App* app)
    : View(),
      game_(game),
      app_(app),
      unit_info_view_(nullptr),
      unit_view_(nullptr),
      control_view_(nullptr),
      dialog_view_(nullptr),
      magic_list_view_(nullptr),
      ui_state_machine_(new StateUIView({game, this})),
      mouse_coords_(-1, -1),
      camera_coords_(0, 0),
      max_camera_coords_(size),
      frame_count_(-1) {
  const Rect frame = {0, 0, size.x, size.y};
  SetFrame(&frame);

  // Calculate max_camera_coords_
  const int kMapWidth     = game_->GetMapSize().x * App::kBlockSize;
  const int kMapHeight    = game_->GetMapSize().y * App::kBlockSize;
  const int kWindowWidth  = GetFrame()->GetW();
  const int kWindowHeight = GetFrame()->GetH();
  const int kMaxCameraX   = kMapWidth - kWindowWidth;
  const int kMaxCameraY   = kMapHeight - kWindowHeight;
  max_camera_coords_ = {kMaxCameraX, kMaxCameraY};

  // Create UI views

  Rect unit_frame = LayoutHelper::CalcPosition(GetFrameSize(),
                                               {220, 320},
                                               LayoutHelper::kAlignRgtTop,
                                               LayoutHelper::kDefaultSpace);
  unit_view_ = new UnitView(&unit_frame);
//  unit_view_->SetVisible(false);

  Rect control_frame = LayoutHelper::CalcPosition(GetFrameSize(),
                                                {300, 150},
                                                LayoutHelper::kAlignRgtBot,
                                                LayoutHelper::kDefaultSpace);
  control_view_ = new ControlView(&control_frame, game_, this);

  Rect dialog_frame = *GetFrame();
  dialog_view_ = new ModalDialogView(&dialog_frame, "Put your message here!");
  dialog_view_->SetVisible(false);

  Rect magic_list_frame = {8, 8, 150, 200};
  MagicListView* mlv = new MagicListView(&magic_list_frame);
  mlv->SetMouseUpHandler(game, this);
  mlv->SetVisible(false);
  magic_list_view_ = mlv;

  // Render UnitInfoView
  Rect unit_info_frame = LayoutHelper::CalcPosition(GetFrameSize(),
                                                    {200, 100},
                                                    LayoutHelper::kAlignLftBot,
                                                    LayoutHelper::kDefaultSpace);
  unit_info_view_ = new UnitInfoView(&unit_info_frame);

  Rect unit_dialog_frame = *GetFrame();
  unit_dialog_view_ = new UnitDialogView(&unit_dialog_frame);
  unit_dialog_view_->SetVisible(false);
}

RootView::~RootView() {
  delete unit_info_view_;
  delete unit_view_;
  delete control_view_;
  delete dialog_view_;
  delete unit_dialog_view_;
  delete magic_list_view_;
}

void RootView::Update() {
  GetCurrentState()->Update();
  control_view_->Update();
  frame_count_++;
}

void RootView::RenderView(Drawer* drawer) {
  drawer->SetOffset(camera_coords_);

  // Render Background
  const int kScreenWidth = drawer->GetWindowSize().x;
  const int kScreenHeight = drawer->GetWindowSize().y;
  string path = game_->GetMapBitmapPath();
  TextureManager* tm = drawer->GetTextureManager();
  Texture* background = tm->FetchTexture(path);
  Rect src_rect = {0, 0, kScreenWidth, kScreenHeight};
  Rect dst_rect = {0, 0, kScreenWidth, kScreenHeight};
  drawer->CopyTextureBackground(background, &src_rect, &dst_rect);

  // Render by UI State
  GetCurrentState()->Render(drawer);

  // Render units
  game_->ForEachUnit([this, drawer] (Unit* unit) {
    if (unit == NULL) return;
    if (unit->IsNoRender()) return;

    SpriteType stype = unit->IsHPLow() ? kSpriteLowHP : kSpriteMove;
    int sprite_no = this->GetCurrentSpriteNo(2, App::kMaxFPS / 2);
    SpriteEffect sprite_effect = {kEffectNone, 0};
    if (unit->IsDoneAction()) {
      stype = kSpriteStand;
      sprite_no = 0;
      sprite_effect = {kEffectShade, 128};
    }
    drawer->CopySprite(unit->GetBitmapPath(),
                   stype,
                   unit->GetDirection(),
                   sprite_no,
                   sprite_effect,
                   unit->GetCoords());
  });

  unit_view_->Render(drawer);
  unit_info_view_->Render(drawer);
  control_view_->Render(drawer);
  magic_list_view_->Render(drawer);
  dialog_view_->Render(drawer);
  unit_dialog_view_->Render(drawer);
}

bool RootView::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (unit_dialog_view_->DelegateMouseButtonEvent(e)) return true;
  if (dialog_view_->DelegateMouseButtonEvent(e)) return true;
  if (magic_list_view_->DelegateMouseButtonEvent(e)) return true;
  if (control_view_->DelegateMouseButtonEvent(e)) return true;
  if (unit_view_->DelegateMouseButtonEvent(e)) return true;
  return GetCurrentState()->OnMouseButtonEvent(e);
}

bool RootView::OnMouseMotionEvent(const MouseMotionEvent e) {
  mouse_coords_ = e.GetCoords();

  if (unit_dialog_view_->DelegateMouseMotionEvent(e)) return true;
  if (dialog_view_->DelegateMouseMotionEvent(e)) return true;
  if (magic_list_view_->DelegateMouseMotionEvent(e)) return true;
  if (control_view_->DelegateMouseMotionEvent(e)) return true;
  if (unit_view_->DelegateMouseMotionEvent(e)) return true;
  if (GetCurrentState()->OnMouseMotionEvent(e)) return true;

  return true;
}

void RootView::MoveCameraX(int d) {
  camera_coords_.x += d;
  if (camera_coords_.x < 0)
    camera_coords_.x = 0;
  if (camera_coords_.x > max_camera_coords_.x)
    camera_coords_.x = max_camera_coords_.x;
}

void RootView::MoveCameraY(int d) {
  camera_coords_.y += d;
  if (camera_coords_.y < 0)
    camera_coords_.y = 0;
  if (camera_coords_.y > max_camera_coords_.y)
    camera_coords_.y = max_camera_coords_.y;
}

void RootView::CenterCamera(Vec2D coords) {
  coords -= (GetFrameSize() / 2);
  camera_coords_ = coords.Bound({0, 0}, max_camera_coords_);
}

void RootView::RaiseMouseMotionEvent() {
  Vec2D mouse_pos;
  SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y); // FIXME Not to use SDL interface directly
  OnMouseMotionEvent(MouseMotionEvent(MouseMotionEvent::kMotionOver, mouse_pos, mouse_pos));
}

void RootView::ChangeUIState(StateUI* state_ui) {
  ui_state_machine_.ChangeState(state_ui);
  RaiseMouseMotionEvent();
}

void RootView::PushUIState(StateUI* state_ui) {
  ui_state_machine_.PushState(state_ui);
  RaiseMouseMotionEvent();
}

void RootView::PopUIState() {
  ui_state_machine_.PopState();
  RaiseMouseMotionEvent();
}

void RootView::InitUIStateMachine() {
  ui_state_machine_.InitState();
  if (game_->HasPendingCmd()) {
    ui_state_machine_.PushState(new StateUIDoCmd({game_, this}));
  }
}

int RootView::GetCurrentSpriteNo(int num_sprites, int frames_per_sprite) const {
  return (frame_count_ / frames_per_sprite) % num_sprites;
}

void RootView::ShowMagicListView(Unit* unit) {
  magic_list_view_->SetUnitAndMagicList(unit, new MagicList(game_->GetMagicManager(), unit));
  magic_list_view_->SetVisible(true);
}

void RootView::HideMagicListView() {
  magic_list_view_->SetVisible(false);
}

void RootView::SetUnitViewVisible(bool b) {
  unit_view_->SetVisible(b);
}

void RootView::SetUnitViewUnit(Unit* unit) {
  unit_view_->SetUnit(unit);
}

void RootView::SetUnitInfoViewVisible(bool b) {
  unit_info_view_->SetVisible(b);
}

void RootView::SetUnitInfoViewUnitTerrainInfo(Cell* cell) {
  unit_info_view_->SetUnitTerrainInfo(cell);
}

void RootView::SetDialogViewVisible(bool b) {
  dialog_view_->SetVisible(b);
}

void RootView::SetDialogViewText(const string& s) {
  dialog_view_->SetText(s);
}

void RootView::SetUnitDialogViewVisible(bool b) {
  unit_dialog_view_->SetVisible(b);
}

bool RootView::IsUnitDialogViewVisible() {
  return unit_dialog_view_->IsVisible();
}

void RootView::SetUnitDialogViewText(const string& s) {
  unit_dialog_view_->SetText(s);
}

void RootView::SetUnitDialogViewUnit(Unit* u) {
  unit_dialog_view_->SetUnit(u);
}

void RootView::EndGame() {
  app_->EndGame();
}


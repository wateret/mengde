#include "root_view.h"

#include "app.h"
#include "control_view.h"
#include "core/scenario.h"
#include "core/stage.h"
#include "deploy_view.h"
#include "game_view.h"
#include "gui/uifw/drawer.h"
#include "gui/uifw/layout_helper.h"
#include "layout_helper.h"
#include "ui_views.h"

namespace mengde {
namespace gui {
namespace app {

RootView::RootView(const Rect& frame, core::Scenario* scenario, App* app)
    : View(frame), app_(app), game_view_(nullptr), ui_views_(nullptr) {
  game_view_ = new GameView(frame, scenario->GetStage(), app_);

  ui_views_ = new UIViews(frame, scenario, game_view_);
  ui_views_->SetTransparent();

  game_view_->SetUIViews(ui_views_);
}

RootView::~RootView() {
  // FIXME ui_view_ is deleted first then StateMachine destructs and it could do something with ui_views_ then
  //       we are doomed.
  delete game_view_;
  delete ui_views_;
}

void RootView::Update() {
  game_view_->Update();
  ui_views_->Update();
}

void RootView::Render(Drawer* drawer) {
  game_view_->Render(drawer);
  ui_views_->Render(drawer);
}

bool RootView::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  return ui_views_->OnMouseButtonEvent(e) || game_view_->OnMouseButtonEvent(e);
}

bool RootView::OnMouseMotionEvent(const foundation::MouseMotionEvent& e) {
  if (ui_views_->OnMouseMotionEvent(e)) return true;
  if (game_view_->OnMouseMotionEvent(e)) return true;

  return true;
}

bool RootView::OnMouseWheelEvent(const foundation::MouseWheelEvent& e) {
  if (ui_views_->DelegateMouseWheelEvent(e)) return true;
  if (game_view_->OnMouseWheelEvent(e)) return true;

  return true;
}

bool RootView::OnKeyEvent(const foundation::KeyEvent& e) {
  if (ui_views_->DelegateKeyEvent(e)) return true;
  if (game_view_->OnKeyEvent(e)) return true;

  return true;
}

void RootView::RaiseMouseOverEvent() {
  Vec2D mouse_pos;
  SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);  // FIXME Not to use SDL interface directly
  OnMouseMotionEvent(foundation::MouseMotionEvent(MouseMotionEvent::Type::kOver, mouse_pos, mouse_pos));
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

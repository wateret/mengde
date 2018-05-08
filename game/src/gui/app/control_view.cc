#include "control_view.h"

#include "config.h"
#include "core/cmd.h"
#include "core/game.h"
#include "game_view.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/text_view.h"
#include "minimap_view.h"

namespace mengde {
namespace gui {
namespace app {

ControlView::ControlView(const Rect* rect, core::Game* game, GameView* gv) : CompositeView(rect), game_(game) {
  bg_color(COLOR("darkgray", 192));
  padding(8);

  Rect frame_tv_turn = {0, 0, 100, 22};
  tv_turn_           = new TextView(&frame_tv_turn);
  SetTurnText(game_->GetTurnCurrent(), game_->GetTurnLimit());
  AddChild(tv_turn_);

  Rect button_coords = {0, 30, 100, 20};
  btn_end_turn_      = new ButtonView(&button_coords, "EndTurn");
  btn_end_turn_->SetMouseButtonHandler([this, gv](const foundation::MouseButtonEvent& e) {
    if (e.IsLeftButtonUp()) {
      // TODO Handle clicked twice in a frame
      this->SetEndTurnVisible(false);
      this->game_->Push(unique_ptr<core::CmdEndTurn>(new core::CmdEndTurn()));
      gv->InitUIStateMachine();
      return true;
    }
    return true;
  });
  AddChild(btn_end_turn_);

  const Vec2D map_size = game_->GetMapSize() * config::kBlockSize;

  const Vec2D minimap_max_size(184, 120);
  const Vec2D minimap_size = LayoutHelper::CalcFittedSize(map_size, minimap_max_size);
  Rect minimap_frame       = LayoutHelper::CalcPosition(GetActualFrameSize(), minimap_size, LayoutHelper::kAlignRgtMid);
  MinimapView* minimap_view =
      new MinimapView(&minimap_frame, game, gv->GetCameraCoordsPtr(), gv->GetFrameSize(), map_size);
  AddChild(minimap_view);
}

void ControlView::SetTurnText(int cur_turn, int max_turn) {
  tv_turn_->SetText(string("Turn ") + std::to_string(cur_turn) + string("/") + std::to_string(max_turn));
}

void ControlView::SetEndTurnVisible(bool b) { btn_end_turn_->visible(b); }

}  // namespace app
}  // namespace gui
}  // namespace mengde

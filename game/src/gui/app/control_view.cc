#include "control_view.h"

#include "config.h"
#include "core/cmd.h"
#include "core/save_file.h"
#include "core/scenario.h"
#include "core/stage.h"
#include "game_view.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/text_view.h"
#include "minimap_view.h"

namespace mengde {
namespace gui {
namespace app {

ControlView::ControlView(const Rect& rect, core::Scenario* sce, GameView* gv) : CompositeView(rect), sce_(sce) {
  bg_color(COLOR("darkgray", 192));
  padding(8);

  Rect frame_tv_turn = {0, 0, 100, 22};
  tv_turn_ = new TextView(frame_tv_turn);
  const auto& turn = sce_->current_stage()->GetTurn();
  SetTurnText(turn.current(), turn.limit());
  AddChild(tv_turn_);

  Rect button_coords = {0, 30, 100, 20};
  btn_end_turn_ = new ButtonView(&button_coords, "EndTurn");
  btn_end_turn_->SetMouseButtonHandler([this, gv](const foundation::MouseButtonEvent& e) {
    if (e.IsLeftButtonUp()) {
      // TODO Handle clicked twice in a frame
      this->SetEndTurnVisible(false);
      this->sce_->current_stage()->Push(std::make_unique<core::CmdEndTurn>());
      gv->InitUIStateMachine();
      return true;
    }
    return true;
  });
  AddChild(btn_end_turn_);

  button_coords.Move(0, 28);
  btn_save_ = new ButtonView(&button_coords, "Save");
  btn_save_->SetMouseButtonHandler([this](const foundation::MouseButtonEvent& e) {
    if (e.IsLeftButtonUp()) {
      core::SaveFile save{Path{"save.mengde"}};
      save.Serialize(*sce_);
      LOG_INFO("File saved");
      return true;
    }
    return true;
  });
  AddChild(btn_save_);

  const Vec2D map_size = sce_->current_stage()->GetMapSize() * config::kBlockSize;

  const Vec2D minimap_max_size(184, 120);
  const Vec2D minimap_size = LayoutHelper::CalcFittedSize(map_size, minimap_max_size);
  Rect minimap_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), minimap_size, LayoutHelper::kAlignRgtMid);
  MinimapView* minimap_view =
      new MinimapView(minimap_frame, sce_->current_stage(), gv->GetCameraCoordsPtr(), gv->GetFrameSize(), map_size);
  AddChild(minimap_view);
}

void ControlView::SetTurnText(int cur_turn, int max_turn) {
  tv_turn_->SetText(string("Turn ") + std::to_string(cur_turn) + string("/") + std::to_string(max_turn));
}

void ControlView::SetEndTurnVisible(bool b) { btn_end_turn_->visible(b); }

}  // namespace app
}  // namespace gui
}  // namespace mengde

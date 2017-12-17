#include "control_view.h"
#include "button_view.h"
#include "minimap_view.h"
#include "text_view.h"
#include "core/cmd.h"
#include "core/game.h"
#include "view/root_view.h"

ControlView::ControlView(const Rect* rect, Game* game, RootView* rv)
    : CompositeView(rect), game_(game), rv_(rv) {
  SetBgColor(COLOR("darkgray", 192));
  SetPadding(8);

  Rect frame_tv_turn = {0, 0, 100, 22};
  TextView* tv_turn = new TextView(&frame_tv_turn, "Turn 10/20");
  AddChild(tv_turn);

  Rect button_coords = {0, 30, 100, 20};
  ButtonView* button = new ButtonView(&button_coords, "EndTurn");
  button->SetMouseButtonHandler([this] (const MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      this->game_->PushCmd(unique_ptr<CmdEndTurn>(new CmdEndTurn()));
      return true;
    }
    return true;
  });
  AddChild(button);

  const Vec2D map_size = game_->GetMapSize() * App::kBlockSize;

  const Vec2D minimap_max_size(184, 120);
  const Vec2D minimap_size = LayoutHelper::CalcFittedSize(map_size, minimap_max_size);
  Rect minimap_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), minimap_size, LayoutHelper::kAlignRgtMid);
  MinimapView* minimap_view = new MinimapView(&minimap_frame,
                                              game,
                                              rv->GetCameraCoordsPtr(),
                                              rv->GetFrameSize(),
                                              map_size);
  AddChild(minimap_view);
}


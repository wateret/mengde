#include "control_view.h"
#include "button_view.h"
#include "core/cmd.h"
#include "core/game.h"
#include "view/root_view.h"
#include "minimap_view.h"

ControlView::ControlView(const Rect* rect, Game* game, RootView* rv)
    : CompositeView(rect), game_(game), rv_(rv) {
  SetBgColor(COLOR_DARKGRAY);
  SetPadding(0);
  Rect button_coords = {0, 30, 80, 20};
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
  const int minimap_height = 120;
  const int minimap_width = minimap_height * map_size.x / map_size.y;
  Rect minimap_frame = Rect(80+8, 0, minimap_width, minimap_height);
  MinimapView* minimap_view = new MinimapView(&minimap_frame,
                                              game,
                                              rv->GetCameraCoordsPtr(),
                                              rv->GetFrameSize(),
                                              map_size);
  AddChild(minimap_view);
}


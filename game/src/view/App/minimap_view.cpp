#include "minimap_view.h"
#include "core/game.h"
#include "view/Foundation/view.h"
#include "view/Foundation/layout_helper.h"
#include "view/Foundation/drawer.h"
#include "view/UIFW/rect_view.h"
#include "view/UIFW/image_view.h"

MinimapView::MinimapView(const Rect* frame,
                         Game* game,
                         Vec2D* camera_coords_ptr,
                         Vec2D camera_size,
                         Vec2D map_size)
    : CompositeView(frame),
      camera_coords_ptr_(camera_coords_ptr),
      camera_size_(camera_size),
      map_size_(map_size) {
  SetPadding(0);

  Rect bg_frame = GetActualFrame();
  bg_image_view_ = new ImageView(&bg_frame, game->GetMapBitmapPath());
  AddChild(bg_image_view_);

  mini_units_view_ = new MinimapUnitsView(&bg_frame, game, map_size);
  AddChild(mini_units_view_);

  Vec2D frame_size = GetFrameSize();
  Vec2D camera_area_size = frame_size * camera_size / map_size;
  Rect rect_frame(MagnifyForMinimap(*camera_coords_ptr), camera_area_size);
  camera_rect_view_ = new RectView(&rect_frame, COLOR("transparent"));
  camera_rect_view_->SetBorder(2, COLOR("white"));
  AddChild(camera_rect_view_);
}

void MinimapView::Update() {
  camera_rect_view_->SetCoords(MagnifyForMinimap(*camera_coords_ptr_));
}

Vec2D MinimapView::MagnifyForMinimap(Vec2D v) {
  return v * GetFrameSize() / map_size_;
}

MinimapUnitsView::MinimapUnitsView(const Rect* frame, Game* game, Vec2D map_size) : View(frame), game_(game) {
  unit_size_ = {48, 48};
  unit_size_ = unit_size_ * GetFrameSize() / map_size;
}

void MinimapUnitsView::Render(Drawer* drawer) {
  game_->ForEachUnit([=] (Unit* u) {
    switch (u->GetForce()) {
      case Force::kOwn:
        drawer->SetDrawColor({192, 0, 0, 255});
        break;
      case Force::kAlly:
        drawer->SetDrawColor(COLOR("orange"));
        break;
      case Force::kEnemy:
        drawer->SetDrawColor(COLOR("blue"));
        break;
      default:
        LOG_FATAL("Unknown force for a unit.");
        drawer->SetDrawColor(COLOR("gray"));
        break;
      }

    Rect rect(u->GetPosition(), {1, 1});
    rect.Magnify(unit_size_);
    rect -= 1;
    drawer->FillRect(&rect);
  });
}

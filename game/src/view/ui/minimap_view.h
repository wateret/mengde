#ifndef MINIMAP_VIEW_H_
#define MINIMAP_VIEW_H_

#include "composite_view.h"
#include "util/common.h"

class RectView;
class ImageView;
class MinimapUnitsView;
class Game;

class MinimapView : public CompositeView {
 public:
  MinimapView(const Rect*, Game*, Vec2D*, Vec2D, Vec2D);
  virtual void Update() override;

 private:
  Vec2D MagnifyForMinimap(Vec2D);

 private:
  Game*  game_;
  Vec2D* camera_coords_ptr_; // FIXME not a good design to have this as a pointer
  Vec2D  camera_size_;
  Vec2D  map_size_;
  RectView* camera_rect_view_;
  ImageView* bg_image_view_;
  MinimapUnitsView* mini_units_view_;
};

class MinimapUnitsView : public View {
 public:
  MinimapUnitsView(const Rect*, Game*, Vec2D);
  virtual void RenderView(Drawer*) override;

 private:
  Game* game_;
  Vec2D unit_size_;
};

#endif // MINIMAP_VIEW_H_

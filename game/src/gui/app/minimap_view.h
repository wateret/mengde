#ifndef MENGDE_GUI_APP_MINIMAP_VIEW_H_
#define MENGDE_GUI_APP_MINIMAP_VIEW_H_

#include "gui/uifw/composite_view.h"
#include "util/common.h"

#include "common.h"

namespace mengde {
namespace core {
class Game;
}
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class MinimapUnitsView;

class MinimapView : public CompositeView {
 public:
  MinimapView(const Rect*, core::Game*, Vec2D*, Vec2D, Vec2D);
  virtual void Update() override;

 private:
  Vec2D MagnifyForMinimap(Vec2D);

 private:
  Vec2D*            camera_coords_ptr_;  // FIXME bad design - Maybe use observer pattern? (Could be overkill)
  Vec2D             camera_size_;
  Vec2D             map_size_;
  RectView*         camera_rect_view_;
  ImageView*        bg_image_view_;
  MinimapUnitsView* mini_units_view_;
};

class MinimapUnitsView : public View {
 public:
  MinimapUnitsView(const Rect*, core::Game*, Vec2D);
  virtual void Render(Drawer*) override;

 private:
  core::Game* game_;
  Vec2D       unit_size_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_MINIMAP_VIEW_H_

#include "terrain_info_view.h"
#include "app.h"
#include "drawer.h"

TerrainInfoView::TerrainInfoView(const Rect& frame,
                                 Vec2D coords,
                                 Terrain* terrain)
    : View(frame), coords_(coords), terrain_(terrain) {
  SetBgColor(COLOR("darkgray"));
  SetPadding(8);
}

void TerrainInfoView::Render(Drawer* drawer) {
  std::string str_name = terrain_->GetName();
  drawer->DrawText(str_name, 14, {255, 255, 255, 255}, {0, 0});
}

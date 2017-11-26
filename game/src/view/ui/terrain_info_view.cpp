#include "terrain_info_view.h"
#include "app.h"
#include "drawer.h"

TerrainInfoView::TerrainInfoView(const Rect* frame,
                                 Vec2D coords,
                                 Terrain* terrain)
    : View(frame), coords_(coords), terrain_(terrain) {
  SetBgColor(COLOR_DARKGRAY);
  SetPadding(8);
}

void TerrainInfoView::RenderView(Drawer* drawer) {
  std::string str_name = terrain_->GetName();
  drawer->DrawTextAbs(str_name, 14, {255, 255, 255, 255}, {0, 0});
}

#include "terrain_info_view.h"

#include "gui/uifw/drawer.h"
#include "gui/uifw/text_view.h"

TerrainInfoView::TerrainInfoView(const Rect& frame,
                                 Vec2D coords)
    : CompositeView(frame), coords_(coords) {
  bg_color(COLOR("darkgray"));
  padding(8);

  Rect name_frame(GetActualFrame());
  tv_name_ = new TextView(&name_frame);
  AddChild(tv_name_);
}

void TerrainInfoView::SetText(const string& s) {
  tv_name_->SetText(s);
}

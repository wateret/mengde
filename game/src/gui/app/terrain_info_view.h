#ifndef TERRAIN_INFO_VIEW_H_
#define TERRAIN_INFO_VIEW_H_

#include "core/terrain.h"
#include "gui/uifw/composite_view.h"
#include "util/common.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace app {

class TerrainInfoView : public CompositeView {
 public:
  TerrainInfoView(const Rect&, Vec2D);
  void SetText(const string&);

 private:
  Vec2D     coords_;
  TextView* tv_name_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // TERRAIN_INFO_VIEW_H_

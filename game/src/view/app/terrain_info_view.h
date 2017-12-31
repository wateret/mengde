#ifndef TERRAIN_INFO_VIEW_H_
#define TERRAIN_INFO_VIEW_H_

#include "view/uifw/composite_view.h"
#include "util/common.h"
#include "core/terrain.h"

class TextView;

class TerrainInfoView : public CompositeView {
 public:
  TerrainInfoView(const Rect&, Vec2D);
  void SetText(const string&);

 private:
  Vec2D coords_;
  TextView* tv_name_;
};

#endif // TERRAIN_INFO_VIEW_H_

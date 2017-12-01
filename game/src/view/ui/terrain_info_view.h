#ifndef TERRAIN_INFO_VIEW_H_
#define TERRAIN_INFO_VIEW_H_

#include "view.h"
#include "util/common.h"
#include "core/terrain.h"

class TerrainInfoView : public View {
 public:
  TerrainInfoView(const Rect*, Vec2D, Terrain*);
  virtual void Render(Drawer*) override;

 private:
  Vec2D coords_;
  Terrain* terrain_;
};

#endif // TERRAIN_INFO_VIEW_H_

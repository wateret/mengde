#ifndef MENGDE_CORE_MAP_H_
#define MENGDE_CORE_MAP_H_

#include "resource_manager.h"
#include "util/common.h"

namespace mengde {
namespace core {

class Terrain;
class Cell;
class Unit;
class PathTree;

class Map {
 public:
  Map(const vector<string>&, const string&, TerrainManager*);
  ~Map();

 public:
  string GetModelId();
  Vec2D GetSize() { return size_; }
  Cell* GetCell(int, int);
  Cell* GetCell(Vec2D);
  bool UnitInCell(Vec2D) const;
  const Unit* GetUnit(Vec2D) const;
  Terrain* GetTerrain(Vec2D);
  int ApplyTerrainEffect(const Unit*, int);
  void EmptyCell(Vec2D);
  void MoveUnit(Vec2D, Vec2D);
  void RemoveUnit(Vec2D);
  PathTree* FindMovablePath(const Unit*);
  vector<Vec2D> FindPathTo(const Unit*, Vec2D);
  int SerializeVec2D(Vec2D v) { return v.y * size_.x + v.x; }
  Vec2D DeserializeVec2D(int v) { return Vec2D(v % size_.x, v / size_.x); }
  void PlaceUnit(const Unit*, Vec2D);
  bool IsHostileAdjacent(const Unit*, Vec2D) const;
  bool IsHostilePlaced(const Unit*, Vec2D) const;
  bool IsValidCoords(Vec2D) const;

 private:
  PathTree* FindPath(const Unit*, Vec2D);

 private:
  Vec2D size_;
  vector<vector<Cell*> > grid_;
  string bitmap_path_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_MAP_H_

#ifndef MENGDE_CORE_MAP_H_
#define MENGDE_CORE_MAP_H_

#include "resource_manager.h"
#include "util/common.h"

#include "user_interface.h"

namespace mengde {
namespace core {

class Terrain;
class Cell;
class Unit;
class PathTree;

class Map {
 public:
  Map(const UserInterface* ui, const vector<string>&, const string&, TerrainManager*);
  ~Map();

 public:
  string GetModelId();
  Vec2D GetSize() { return size_; }
  Cell* GetCell(int, int);
  Cell* GetCell(Vec2D);
  bool UnitInCell(Vec2D) const;
  UId GetUnitId(Vec2D) const;
  Terrain* GetTerrain(Vec2D);
  int ApplyTerrainEffect(const Unit*, int);
  void EmptyCell(Vec2D);
  void MoveUnit(Vec2D, Vec2D);
  void RemoveUnit(Vec2D);
  PathTree* FindMovablePath(const UId&);
  vector<Vec2D> FindPathTo(const UId&, Vec2D);
  int SerializeVec2D(Vec2D v) { return v.y * size_.x + v.x; }
  Vec2D DeserializeVec2D(int v) { return Vec2D(v % size_.x, v / size_.x); }
  void PlaceUnit(const UId&, Vec2D);
  bool IsHostileAdjacent(const UId&, Vec2D) const;
  bool IsHostilePlaced(const UId&, Vec2D) const;
  bool IsValidCoords(Vec2D) const;

 private:
  PathTree* FindPath(const UId&, Vec2D);

 private:
  const UserInterface* ui_;
  Vec2D size_;
  vector<vector<Cell*> > grid_;
  string bitmap_path_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_MAP_H_

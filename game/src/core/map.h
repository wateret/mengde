#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <string>
#include "util/common.h"
#include "resource_manager.h"

class PathTree;
class Terrain;
class Cell;
class Unit;

class Map {
 public:
  Map(const vector<string>&, const string&, TerrainManager*);
  ~Map();

 public:
  string GetBitmapPath();
  Vec2D GetSize() { return size_; }
  Cell* GetCell(int, int);
  Cell* GetCell(Vec2D);
  bool UnitInCell(Vec2D) const;
  Unit* GetUnit(Vec2D);
  Terrain* GetTerrain(Vec2D);
  int ApplyTerrainEffect(Unit*, int);
  void EmptyCell(Vec2D);
  void MoveUnit(Vec2D, Vec2D);
  void RemoveUnit(Vec2D);
  PathTree* FindMovablePath(Unit*);
  vector<Vec2D> FindPathTo(Unit*, Vec2D);
  int SerializeVec2D(Vec2D v) { return v.y * size_.x + v.x; }
  Vec2D DeserializeVec2D(int v) { return Vec2D(v % size_.x, v / size_.x); }
  void PlaceUnit(Unit*, Vec2D);
  bool IsHostileAdjacent(Unit*, Vec2D);
  bool IsHostilePlaced(Unit*, Vec2D);
  bool IsValidCoords(Vec2D) const;

 private:
  PathTree* FindPath(Unit*, Vec2D);

 private:
  Vec2D size_;
  vector< vector<Cell*> > grid_;
  string bitmap_path_;
};

#endif // MAP_H_

#include "map.h"

#include <algorithm>

#include "util/common.h"
#include "core/path_tree.h"
#include "unit_class.h"
#include "cell.h"
#include "unit.h"

namespace mengde {
namespace core {

Map::Map(const vector<string>& input,
         const string& bitmap_path,
         TerrainManager* tm)
    : bitmap_path_(bitmap_path) {
  int rows = input.size();
  int cols = input[0].size();
  size_ = {cols, rows};

  grid_.resize(rows);
  for (int i = 0; i < rows; i++) {
    grid_[i].resize(cols);
    for (int j = 0; j < cols; j++) {
      Terrain* terrain = tm->Get(string(1, input[i][j]));
      ASSERT(terrain != NULL);
      grid_[i][j] = new Cell(terrain);
    }
  }
}

Map::~Map() {
  for (int i = 0; i < size_.y; i++) {
    for (int j = 0; j < size_.x; j++) {
      delete grid_[i][j];
    }
  }
}

string Map::GetModelId() {
  return bitmap_path_;
}

Cell* Map::GetCell(int c, int r) {
  ASSERT(IsValidCoords({c, r}));
  return grid_[r][c];
}

Cell* Map::GetCell(Vec2D v) {
  ASSERT(IsValidCoords(v));
  return grid_[v.y][v.x];
}

bool Map::UnitInCell(Vec2D c)  const {
  return IsValidCoords(c) && grid_[c.y][c.x]->IsUnitPlaced();
}

Unit* Map::GetUnit(Vec2D c) {
  ASSERT(UnitInCell(c));
  return grid_[c.y][c.x]->GetUnit();
}

void Map::RemoveUnit(Vec2D c) {
  ASSERT(UnitInCell(c));
  grid_[c.y][c.x]->Empty();
}

Terrain* Map::GetTerrain(Vec2D c) {
  ASSERT(IsValidCoords(c));
  return grid_[c.y][c.x]->GetTerrain();
}

// Using Dijkstra Shortest Path Algorithm
// ( O(N^2) where N is number of vertices )
PathTree* Map::FindPath(Unit* unit, Vec2D dest) {
  static const int kDNum = 4;
  static const int kDRow[] = {0, 0, -1, 1};
  static const int kDCol[] = {-1, 1, 0, 0};
  const int N = size_.x * size_.y;
  const int kInf = 1 << 30;

  vector<int> dist(N, kInf);
  vector<bool> used(N, false);
  vector<PathTreeNode*> from(N, NULL);
  Vec2D coords = unit->GetPosition();
  int sc = SerializeVec2D(coords);
  int stat_move = kInf;
  if (dest == Vec2D(-1, -1)) {
    stat_move = unit->GetMove();
  }
  dist[sc] = 0;

  PathTree* pathtree = NULL;

  while (true) {
    int current = -1;

    // FIXME Optimization
    // Closest node selection is currently O(N)
    // This can be done in O(logn) with priority queue
    // Or it may be possible to do this with BFS
    for (int i = 0; i < N; i++) {
      if (used[i]) continue;
      if (current == -1 || dist[current] > dist[i]) {
        current = i;
      }
    }
    if (current == -1) break;

    Vec2D vec_current = DeserializeVec2D(current);
    used[current] = true;
    if (dist[current] <= stat_move && !IsHostilePlaced(unit, vec_current)) {
      if (pathtree == nullptr) {
        pathtree = new PathTree(vec_current);
        from[current] = pathtree->GetRoot();
      } else {
        from[current] = pathtree->Adopt(vec_current, from[current]);
        if (vec_current == dest) {
          return pathtree;
        }
      }
    } else {
      break;
    }

    for (int i = 0; i < kDNum; i++) {
      int nr = vec_current.y + kDRow[i];
      int nc = vec_current.x + kDCol[i];
      Vec2D nvec(nc, nr);
      if (!IsValidCoords(nvec)) continue;
      int next = SerializeVec2D(nvec);
      int new_dist = dist[current] +
                     grid_[nr][nc]->GetMoveCost(unit->GetClassIndex());

      // handle ZOC
      if (new_dist < stat_move && IsHostileAdjacent(unit, nvec) && nvec != dest) {
        new_dist = stat_move;
      }

      if (dist[next] > new_dist) {
        dist[next] = new_dist;
        from[next] = from[current];
      }
    }
  }

  return pathtree;
}

PathTree* Map::FindMovablePath(Unit* unit) {
  return FindPath(unit, {-1, -1});
}

vector<Vec2D> Map::FindPathTo(Unit* unit, Vec2D dest) {
  unique_ptr<PathTree> pathtree(FindPath(unit, dest));
  vector<Vec2D> path = pathtree->GetPathToRoot(dest);
  return path;
}

void Map::PlaceUnit(Unit* unit, Vec2D c) {
  ASSERT(IsValidCoords(c));
  ASSERT(!grid_[c.y][c.x]->IsUnitPlaced());
  grid_[c.y][c.x]->SetUnit(unit);
  unit->SetPosition(c);

}

void Map::MoveUnit(Vec2D src, Vec2D dst) {
  Unit* unit = GetUnit(src);
  EmptyCell(src);
  PlaceUnit(unit, dst);
}

void Map::EmptyCell(Vec2D c) {
  grid_[c.y][c.x]->Empty();
}

bool Map::IsHostileAdjacent(Unit* unit, Vec2D coords) {
  static const int kDNum = 5;
  static const int kDRow[] = {0, 0, 0, -1, 1};
  static const int kDCol[] = {0, -1, 1, 0, 0};
  for (int i = 0; i < kDNum; i++) {
    int nr = coords.y + kDRow[i];
    int nc = coords.x + kDCol[i];
    if (!IsValidCoords({nc, nr})) continue;
    Vec2D ncoords(nc, nr);
    if (UnitInCell(ncoords)) {
      if (unit->IsHostile(GetUnit(ncoords)))
        return true;
    }
  }
  return false;
}

bool Map::IsHostilePlaced(Unit* unit, Vec2D coords) {
  Cell* cell = grid_[coords.y][coords.x];
  ASSERT(cell != nullptr);
  if (cell->IsUnitPlaced() && unit->IsHostile(cell->GetUnit())) {
    return true;
  }
  return false;
}

bool Map::IsValidCoords(Vec2D c) const {
  return c.x >= 0 && c.x < size_.x && c.y >= 0 && c.y < size_.y;
}

int Map::ApplyTerrainEffect(Unit* unit, int value) {
  Vec2D v = unit->GetPosition();
  return grid_[v.y][v.x]->ApplyTerrainEffect(unit->GetClassIndex(), value);
}

} // namespace core
} // namespace mengde

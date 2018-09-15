#include "map.h"

#include <algorithm>

#include "cell.h"
#include "core/path_tree.h"
#include "hero_class.h"
#include "unit.h"
#include "util/common.h"

namespace mengde {
namespace core {

Map::Map(const UserInterface* ui, const vector<string>& input, const string& bitmap_path, TerrainManager* tm)
    : ui_(ui), bitmap_path_(bitmap_path) {
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

string Map::GetModelId() { return bitmap_path_; }

const Cell* Map::GetCell(int c, int r) const {
  ASSERT(IsValidCoords({c, r}));
  return grid_[r][c];
}

const Cell* Map::GetCell(Vec2D v) const {
  ASSERT(IsValidCoords(v));
  return grid_[v.y][v.x];
}

bool Map::UnitInCell(Vec2D c) const { return IsValidCoords(c) && grid_[c.y][c.x]->IsUnitPlaced(); }

UId Map::GetUnitId(Vec2D c) const {
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

const Terrain* Map::GetTerrain(Vec2D c) const {
  ASSERT(IsValidCoords(c));
  return grid_[c.y][c.x]->GetTerrain();
}

// Using Dijkstra Shortest Path Algorithm
// ( O(N^2) where N is number of vertices )
PathTree* Map::FindPath(const UId& uid, Vec2D dest) {
  static const int kDNum = 4;
  static const int kDRow[] = {0, 0, -1, 1};
  static const int kDCol[] = {-1, 1, 0, 0};
  const int N = size_.x * size_.y;
  const int kInf = 1 << 30;

  const Unit* unit = ui_->GetUnit(uid);

  vector<int> dist(N, kInf);
  vector<bool> used(N, false);
  vector<PathNode*> from(N, NULL);
  Vec2D coords = unit->position();
  int sc = SerializeVec2D(coords);
  int stat_move = kInf;
  if (dest == Vec2D(-1, -1)) {
    stat_move = unit->move();
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

    if (IsHostilePlaced(uid, vec_current)) continue;

    if (dist[current] <= stat_move) {
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
      int new_dist = dist[current] + grid_[nr][nc]->GetMoveCost(unit->class_index());

      // handle ZOC
      if (new_dist < stat_move && IsHostileAdjacent(uid, nvec) && nvec != dest) {
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

PathTree* Map::FindMovablePath(const UId& uid) { return FindPath(uid, {-1, -1}); }

vector<Vec2D> Map::FindPathTo(const UId& uid, Vec2D dest) {
  unique_ptr<PathTree> pathtree(FindPath(uid, dest));
  vector<Vec2D> path = pathtree->GetPathToRoot(dest);
  return path;
}

void Map::PlaceUnit(const UId& uid, Vec2D c) {
  ASSERT(IsValidCoords(c));
  ASSERT(!grid_[c.y][c.x]->IsUnitPlaced());
  grid_[c.y][c.x]->SetUnit(uid);
}

void Map::MoveUnit(Vec2D src, Vec2D dst) {
  auto uid = GetUnitId(src);
  EmptyCell(src);
  PlaceUnit(uid, dst);
}

void Map::EmptyCell(Vec2D c) { grid_[c.y][c.x]->Empty(); }

bool Map::IsHostileAdjacent(const UId& uid, Vec2D coords) const {
  static const int kDNum = 5;
  static const int kDRow[] = {0, 0, 0, -1, 1};
  static const int kDCol[] = {0, -1, 1, 0, 0};

  const Unit* unit = ui_->GetUnit(uid);
  for (int i = 0; i < kDNum; i++) {
    int nr = coords.y + kDRow[i];
    int nc = coords.x + kDCol[i];
    if (!IsValidCoords({nc, nr})) continue;
    Vec2D ncoords(nc, nr);
    if (UnitInCell(ncoords)) {
      if (unit->IsHostile(ui_->GetUnit(ncoords))) return true;
    }
  }
  return false;
}

bool Map::IsHostilePlaced(const UId& uid, Vec2D coords) const {
  Cell* cell = grid_[coords.y][coords.x];
  ASSERT(cell != nullptr);
  auto unit = ui_->GetUnit(uid);
  auto unit_in_cell = cell->GetUnit();
  if (unit_in_cell && unit->IsHostile(ui_->GetUnit(unit_in_cell))) {
    return true;
  }
  return false;
}

bool Map::IsValidCoords(Vec2D c) const { return c.x >= 0 && c.x < size_.x && c.y >= 0 && c.y < size_.y; }

int Map::ApplyTerrainEffect(const Unit* unit, int value) const {
  Vec2D v = unit->position();
  return grid_[v.y][v.x]->ApplyTerrainEffect(unit->class_index(), value);
}

}  // namespace core
}  // namespace mengde

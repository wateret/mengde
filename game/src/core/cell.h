#ifndef MENGDE_CORE_CELL_H_
#define MENGDE_CORE_CELL_H_

#include <boost/optional.hpp>

#include "id.h"
#include "terrain.h"
#include "unit.h"

namespace mengde {
namespace core {

class Cell {
 public:
  Cell(Terrain*);
  Terrain* GetTerrain();
  int GetMoveCost(int);
  int GetTerrainEffect(int) const;
  int ApplyTerrainEffect(int, int);
  std::string GetTerrainId() const;
  bool IsUnitPlaced() const { return !uid_.IsNone(); }
  UId GetUnit() const { return uid_; }
  void SetUnit(const UId& uid) { uid_ = uid; }
  void Empty() { uid_.SetNone(); }

 private:
  Terrain* terrain_;
  UId uid_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_CELL_H_

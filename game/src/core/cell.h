#ifndef CELL_H_
#define CELL_H_

#include "terrain.h"
#include "unit.h"

namespace mengde {
namespace core {

class Cell {
 public:
  Cell(Terrain*);
  Terrain* GetTerrain();
  Unit* GetUnit();
  int GetMoveCost(int);
  int GetTerrainEffect(int);
  int GetTerrainEffectThisCell();
  int ApplyTerrainEffect(int, int);
  std::string GetTerrainName() const;
  bool IsUnitPlaced() const;
  void SetUnit(Unit*);
  void Empty() { unit_ = nullptr; }

 private:
  Terrain* terrain_;
  Unit* unit_;
};

} // namespace core
} // namespace mengde

#endif // CELL_H_

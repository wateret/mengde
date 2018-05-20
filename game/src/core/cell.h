#ifndef MENGDE_CORE_CELL_H_
#define MENGDE_CORE_CELL_H_

#include "terrain.h"
#include "unit.h"

namespace mengde {
namespace core {

class Cell {
 public:
  Cell(Terrain*);
  Terrain* GetTerrain();
  const Unit* GetUnit() const;
  int GetMoveCost(int);
  int GetTerrainEffect(int) const;
  int GetTerrainEffectThisCell() const;
  int ApplyTerrainEffect(int, int);
  std::string GetTerrainName() const;
  bool IsUnitPlaced() const;
  void SetUnit(const Unit*);
  void Empty() { unit_ = nullptr; }

 private:
  Terrain* terrain_;
  const Unit* unit_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_CELL_H_

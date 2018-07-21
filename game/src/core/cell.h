#ifndef MENGDE_CORE_CELL_H_
#define MENGDE_CORE_CELL_H_

#include <boost/optional.hpp>

#include "terrain.h"
#include "unit.h"

namespace mengde {
namespace core {

class Cell {
 public:
  Cell(Terrain*);
  Terrain* GetTerrain();
  boost::optional<uint32_t> GetUnit() const;
  int GetMoveCost(int);
  int GetTerrainEffect(int) const;
  int ApplyTerrainEffect(int, int);
  std::string GetTerrainName() const;
  bool IsUnitPlaced() const;
  void SetUnit(const boost::optional<uint32_t>&);
  void Empty() { unit_ = boost::none; }

 private:
  Terrain* terrain_;
  boost::optional<uint32_t> unit_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_CELL_H_

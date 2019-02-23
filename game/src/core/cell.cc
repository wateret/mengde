#include "cell.h"

namespace mengde {
namespace core {

Cell::Cell(Terrain* terrain) : terrain_(terrain) {}

Terrain* Cell::GetTerrain() { return terrain_; }

int Cell::GetMoveCost(int class_idx) { return terrain_->GetMoveCost(class_idx); }

int Cell::GetTerrainEffect(int class_idx) const { return terrain_->GetEffect(class_idx); }

int Cell::ApplyTerrainEffect(int class_idx, int value) { return value * terrain_->GetEffect(class_idx) / 100; }

std::string Cell::GetTerrainId() const { return terrain_->id(); }

}  // namespace core
}  // namespace mengde

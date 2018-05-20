#include "cell.h"

namespace mengde {
namespace core {

Cell::Cell(Terrain* terrain) : terrain_(terrain), unit_(nullptr) {}

Terrain* Cell::GetTerrain() { return terrain_; }

int Cell::GetMoveCost(int class_idx) { return terrain_->GetMoveCost(class_idx); }

int Cell::GetTerrainEffect(int class_idx) const { return terrain_->GetEffect(class_idx); }

int Cell::GetTerrainEffectThisCell() const {
  ASSERT(IsUnitPlaced());
  return terrain_->GetEffect(unit_->GetClassIndex());
}

int Cell::ApplyTerrainEffect(int class_idx, int value) { return value * terrain_->GetEffect(class_idx) / 100; }

std::string Cell::GetTerrainName() const { return terrain_->GetName(); }

bool Cell::IsUnitPlaced() const { return unit_ != nullptr; }

const Unit* Cell::GetUnit() const { return unit_; }

void Cell::SetUnit(const Unit* unit) { unit_ = unit; }

}  // namespace core
}  // namespace mengde

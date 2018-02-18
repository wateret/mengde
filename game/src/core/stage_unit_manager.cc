#include "stage_unit_manager.h"

#include "unit.h"

namespace mengde {
namespace core {

StageUnitManager::StageUnitManager() : units_() {}

uint32_t StageUnitManager::Deploy(Unit* unit) {
  uint32_t unit_id = units_.size();
  units_.push_back(unit);
  return unit_id;
}

void StageUnitManager::Kill(Unit* unit) { unit->Kill(); }

Unit* StageUnitManager::Get(uint32_t id) {
  ASSERT(id < units_.size());
  return units_[id];
}

void StageUnitManager::ForEach(function<void(Unit*)> fn) { std::for_each(units_.begin(), units_.end(), fn); }

}  // namespace core
}  // namespace mengde

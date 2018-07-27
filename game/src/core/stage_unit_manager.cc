#include "stage_unit_manager.h"

#include "unit.h"

namespace mengde {
namespace core {

StageUnitManager::StageUnitManager() : units_() {}

UId StageUnitManager::Deploy(Unit* unit) {
  UId uid{units_.size()};
  units_.push_back(unit);
  unit->SetUnitId(uid);
  return uid;
}

void StageUnitManager::Kill(Unit* unit) { unit->Kill(); }

Unit* StageUnitManager::Get(const UId& id) {
  ASSERT(id);
  ASSERT_LT(id.Value(), units_.size());
  return units_[id.Value()];
}

void StageUnitManager::ForEach(function<void(Unit*)> fn) { std::for_each(units_.begin(), units_.end(), fn); }

void StageUnitManager::ForEachIdxConst(function<void(uint32_t, const Unit*)> fn) const {
  for (uint32_t i = 0, size = units_.size(); i < size; i++) {
    fn(i, units_[i]);
  }
}

}  // namespace core
}  // namespace mengde

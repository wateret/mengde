#include "stage_unit_manager.h"

#include "unit.h"

namespace mengde {
namespace core {

StageUnitManager::StageUnitManager() : units_() {}

UId StageUnitManager::Deploy(Unit* unit) {
  UId uid{static_cast<uint32_t>(units_.size())};
  units_.push_back(unit);
  unit->uid(uid);
  return uid;
}

void StageUnitManager::Kill(Unit* unit) { unit->Kill(); }

Unit* StageUnitManager::Get(const UId& id) {
  ASSERT(id);
  ASSERT_LT(id.Value(), units_.size());
  return units_[id.Value()];
}

void StageUnitManager::SetAIMode(const UId& id, AIMode mode) { ai_modes_[id] = mode; }

void StageUnitManager::ForEach(function<void(Unit*)> fn) { std::for_each(units_.begin(), units_.end(), fn); }

void StageUnitManager::ForEachConst(function<void(const Unit*)> fn) const {
  for (uint32_t i = 0, size = units_.size(); i < size; i++) {
    fn(units_[i]);
  }
}

}  // namespace core
}  // namespace mengde

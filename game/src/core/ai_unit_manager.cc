#include "ai_unit_manager.h"

namespace mengde {
namespace core {

AIUnitManager::AIUnitManager() : default_{ai_unit_factory_.Get(AIMode::kDoNothing)} {}

void AIUnitManager::Set(const UId& uid, AIMode mode) { map_[uid] = ai_unit_factory_.Get(mode); }

const IAIUnit* AIUnitManager::Get(const UId& uid) const {
  auto itr = map_.find(uid);
  if (itr == map_.end()) {
    return default_;
  }
  return itr->second;
}

}  // namespace core
}  // namespace mengde

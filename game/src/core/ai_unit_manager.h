#ifndef MENGDE_CORE_AI_UNIT_MANAGER_H_
#define MENGDE_CORE_AI_UNIT_MANAGER_H_

#include "ai_mode.h"
#include "ai_unit_factory.h"
#include "id.h"

#include <unordered_map>

namespace mengde {
namespace core {

class AIUnitManager {
 public:
  AIUnitManager();
  void Set(const UId& uid, AIMode mode);
  const IAIUnit* Get(const UId& uid) const;

 private:
  AIUnitFactory ai_unit_factory_;
  std::unordered_map<UId, IAIUnit*> map_;
  IAIUnit* default_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_AI_UNIT_MANAGER_H_

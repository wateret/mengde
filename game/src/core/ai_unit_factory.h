#ifndef MENGDE_CORE_AI_UNIT_FACTORY_H_
#define MENGDE_CORE_AI_UNIT_FACTORY_H_

#include "ai_mode.h"
#include "ai_unit.h"
#include "util/common.h"

#include <unordered_map>

namespace mengde {
namespace core {

// Flyweight Factory for IAIUnit derivatives

class AIUnitFactory {
 public:
  AIUnitFactory();

  IAIUnit* Get(AIMode mode);

 private:
  std::unordered_map<AIMode, unique_ptr<IAIUnit>> flyweight_objects_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_AI_UNIT_FACTORY_H_

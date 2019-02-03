#include "ai_unit_factory.h"

namespace mengde {
namespace core {

AIUnitFactory::AIUnitFactory() {
#define MACRO_AI_MODE(name, str) flyweight_objects_.emplace(AIMode::k##name, std::make_unique<AIUnit##name>());
#include "ai_mode.h.inc"
}

IAIUnit* AIUnitFactory::Get(AIMode mode) {
  auto itr = flyweight_objects_.find(mode);
  if (itr == flyweight_objects_.end()) {
    return nullptr;
  }
  return itr->second.get();
}

}  // namespace core
}  // namespace mengde

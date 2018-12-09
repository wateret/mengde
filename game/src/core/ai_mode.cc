#include "ai_mode.h"

namespace mengde {
namespace core {

AIMode StringToAIMode(const std::string& s) {
#define MACRO_AI_MODE(name, str) \
  if (s == std::string(#str)) return AIMode::k##name;
#include "ai_mode.h.inc"
  return AIMode::kNone;
}

}  // namespace core
}  // namespace mengde

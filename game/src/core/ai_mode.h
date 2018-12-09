#ifndef MENGDE_CORE_AI_MODE_H_
#define MENGDE_CORE_AI_MODE_H_

#include <string>

namespace mengde {
namespace core {

enum class AIMode {
#define MACRO_AI_MODE(name, str) k##name,
#include "ai_mode.h.inc"
  kCount
};

AIMode StringToAIMode(const std::string& s);

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_AI_MODE_H_

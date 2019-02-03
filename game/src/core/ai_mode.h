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

#include <functional>
#include <type_traits>

namespace std {

template <>
struct hash<::mengde::core::AIMode> {
  size_t operator()(::mengde::core::AIMode arg) const {
    using type = std::underlying_type<::mengde::core::AIMode>::type;
    return std::hash<type>{}(static_cast<type>(arg));
  }
};

}  // namespace std

#endif  // MENGDE_CORE_AI_MODE_H_

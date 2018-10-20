#ifndef MENGDE_CORE_CONDITION_H_
#define MENGDE_CORE_CONDITION_H_

namespace mengde {
namespace core {

enum class Condition {
  kNone,      //< None, unused
  kStunned,   //< Cannot do action
  kPoisoned,  //< Get damage over each turn begins
  kRooted     //< Cannot move
};

}  // namespace core
}  // namespace mengde

// NOTE std::hash specialization for enum types should not be necessary since std::hash for enum type is supported as of
//      C++14. However this code is introduced for g++ 5.x which has an issue libstdc++. It was fixed for 6.1 but the
//      compiler version on 16.04 is g++ 5.3.
//      Related : https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60970

#include <functional>
#include <type_traits>

namespace std {
template <>
struct hash<::mengde::core::Condition> {
  size_t operator()(::mengde::core::Condition arg) const {
    using type = std::underlying_type<::mengde::core::Condition>::type;
    return std::hash<type>{}(static_cast<type>(arg));
  }
};
}  // namespace std

#endif  // MENGDE_CORE_CONDITION_H_

#ifndef FORCE_H_
#define FORCE_H_

namespace mengde {
namespace core {

enum class Force {
  kNone     = 0x00,
  kFirst    = 0x01,
  kOwn      = 0x01,
  kAlly     = 0x02,
  kEnemy    = 0x04,
  kFriendly = kOwn | kAlly,
  kLast     = 0x08                   // Not a real value
};

} // namespace core
} // namespace mengde

#endif // FORCE_H_

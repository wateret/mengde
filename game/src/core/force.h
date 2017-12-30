#ifndef FORCE_H_
#define FORCE_H_

enum class Force {
  kNone     = 0x00,
  kFirst    = 0x01,
  kOwn      = 0x01,
  kAlly     = 0x02,
  kEnemy    = 0x04,
  kFriendly = kOwn | kAlly,
  kLast     = 0x08                   // Not a real value
};

#endif // FORCE_H_

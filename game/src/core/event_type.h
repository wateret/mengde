#ifndef EVENT_TYPE_H_
#define EVENT_TYPE_H_

namespace mengde {
namespace core {

enum class EventType {
  kOnNone,
  kOnTurnBegin,
  kOnPreBasicAttack,
  kOnPreBasicAttacked,
  kOnDamaged,
  kOnActionDone
};

} // namespace core
} // namespace mengde

#endif // EVENT_TYPE_H_

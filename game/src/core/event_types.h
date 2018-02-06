#ifndef EVENT_TYPES_H_
#define EVENT_TYPES_H_

namespace mengde {
namespace core {

enum class EventType { // TODO to be removed
  kOnNone,
  kOnTurnBegin,
  kOnPreBasicAttack,
  kOnPreBasicAttacked,
  kOnDamaged,
  kOnActionDone
};

namespace event {

enum class GeneralEvent {
  kNone,
  kTurnBegin,
  kPreBasicAttack,
  kPreBasicAttacked,
  kPostBasicAttack,
  kPostBasicAttacked,
  kDamaged,
  kActionDone
};

enum class OnCmdEvent {
  kNone,
  kBasicAttack,
  kBasicAttacked,
  kDamaged,
  kActionDone
};

} // namespace event

} // namespace core
} // namespace mengde

#endif // EVENT_TYPES_H_

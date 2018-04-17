#ifndef MENGDE_CORE_EVENT_EVENT_TYPES_H_
#define MENGDE_CORE_EVENT_EVENT_TYPES_H_

namespace mengde {
namespace core {
namespace event {

enum class GeneralEvent {
  kNone,
  kTurnBegin,
  kPreNormalAttack,
  kPreNormalAttacked,
  kPostNormalAttack,
  kPostNormalAttacked,
  kPreCounterAttack,
  kPreCounterAttacked,
  kPostCounterAttack,
  kPostCounterAttacked,
  kActionDone
};

enum class OnCmdEvent {
  kNone,
  kNormalAttack,
  kNormalAttacked,
  kCounterAttack,
  kCounterAttacked,
  kDamaged,
  kActionDone
};

}  // namespace event
}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_EVENT_EVENT_TYPES_H_

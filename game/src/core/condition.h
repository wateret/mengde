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

#endif  // MENGDE_CORE_CONDITION_H_

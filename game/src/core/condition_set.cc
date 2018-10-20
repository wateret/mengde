#include "condition_set.h"

#include "util/common.h"

namespace mengde {
namespace core {

bool ConditionSet::Exist(Condition condition) const { return set_.find(condition) != set_.end(); }

TurnBased ConditionSet::Get(Condition condition) const {
  ASSERT(Exist(condition));
  return set_.find(condition)->second;
}

void ConditionSet::Set(Condition condition, const TurnBased& turn_based) { set_.insert({condition, turn_based}); }

void ConditionSet::NextTurn() {
  for (auto& e : set_) {
    e.second.Next();
  }
}

}  // namespace core
}  // namespace mengde

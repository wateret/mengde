#include "condition_set.h"

#include "util/common.h"

namespace mengde {
namespace core {

bool ConditionSet::Has(Condition condition) const { return set_.find(condition) != set_.end(); }

TurnBased ConditionSet::Get(Condition condition) const {
  ASSERT(Has(condition));
  return set_.find(condition)->second;
}

void ConditionSet::Set(Condition condition, const TurnBased& turn_based) { set_.insert({condition, turn_based}); }

void ConditionSet::NextTurn() {
  for (auto it = set_.begin(); it != set_.end();) {
    auto& turns = it->second;
    turns.Next();
    if (turns.left() == 0) {
      it = set_.erase(it);
    } else {
      ++it;
    }
  }
}

void ConditionSet::Iterate(const std::function<void(Condition, TurnBased)>& fn) const {
  for (auto& e : set_) {
    fn(e.first, e.second);
  }
}

}  // namespace core
}  // namespace mengde

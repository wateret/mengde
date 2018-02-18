#include "stat_modifier_list.h"

#include "stat_modifier.h"
#include "util/common.h"

namespace mengde {
namespace core {

StatModifierList::StatModifierList() {}

StatModifierList::~StatModifierList() {
  for (auto e : elements_) {
    delete e;
  }
}

void StatModifierList::AddModifier(StatModifier* m) {
  for (int i = 0, sz = elements_.size(); i < sz; i++) {
    StatModifier* e = elements_[i];
    if (e->GetId() == m->GetId() && e->GetStatId() == m->GetStatId()) {
      // If both two have the same sign it will be replaced
      // or it will be erased (cancelling out)
      delete e;
      if (e->GetMultiplier() * m->GetMultiplier() >= 0) {
        elements_[i] = m;
      } else {
        elements_.erase(elements_.begin() + i);
        delete m;
      }
      return;
    }
  }
  elements_.push_back(m);
}

void StatModifierList::NextTurn() {
  elements_.erase(remove_if(elements_.begin(), elements_.end(), [](StatModifier* m) -> bool {
    bool remove = (m->GetTurnsLeft() == 0);
    if (remove) delete m;
    return remove;
  }));

  for (auto e : elements_) {
    e->NextTurn();
  }
}

Attribute StatModifierList::CalcAddends() const {
  Attribute calc_mods = {0, 0, 0, 0, 0};
  for (auto e : elements_) {
    calc_mods.AddValueByIndex(e->GetStatId(), e->GetAddend());
  }
  return calc_mods;
}

Attribute StatModifierList::CalcMultipliers() const {
  Attribute calc_mods = {0, 0, 0, 0, 0};
  for (auto e : elements_) {
    calc_mods.AddValueByIndex(e->GetStatId(), e->GetMultiplier());
  }
  return calc_mods;
}

}  // namespace core
}  // namespace mengde

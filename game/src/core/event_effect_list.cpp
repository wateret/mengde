#include "event_effect_list.h"
#include "event_effect.h"
#include "common.h"

EventEffectList::EventEffectList() {
}

EventEffectList::~EventEffectList() {
  for (auto e : elements_) {
    delete e;
  }
}

void EventEffectList::AddEffect(EventEffect* e) {
  elements_.push_back(e);
}

void EventEffectList::RaiseEvent(EventEffect::Type type, Unit* unit, void* data) {
  for (auto e : elements_) {
    if (e->IsOfType(type)) {
      e->OnEvent(unit, data);
    }
  }
}

void EventEffectList::NextTurn() {
  elements_.erase(remove_if(elements_.begin(), elements_.end(), [] (EventEffect* e) {
    if (e->GetTurnsLeft() < 0) {
      LOG_WARNING("EventEffect.turns_left_ shouldn't be less than 0");
    }
    bool remove = (e->GetTurnsLeft() <= 0);
    if (remove) delete e;
    return remove;
  }));

  for (auto e : elements_) {
    e->NextTurn();
  }
}

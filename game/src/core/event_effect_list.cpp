#include "event_effect_list.h"

#include "cmd.h" // TODO Only for CmdQueue
#include "event_effect.h"
#include "util/common.h"

namespace mengde {
namespace core {

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

void EventEffectList::AddGeneralEffect(GeneralEventEffect* e) {
  general_elements_.push_back(e);
}

void EventEffectList::AddOnCmdEffect(OnCmdEventEffect* e) {
  oncmd_elements_.push_back(e);
}

void EventEffectList::RaiseEvent(EventType type, Unit* unit) {
  for (auto e : elements_) {
    if (e->type(type)) {
      e->OnEvent(unit);
    }
  }
}

unique_ptr<Cmd> EventEffectList::RaiseEvent(event::GeneralEvent type, Unit* unit) const {
  auto cmdq = unique_ptr<CmdQueue>(new CmdQueue());
  for (auto e : general_elements_) {
    if (e->typeof(type)) {
      *cmdq += e->OnEvent(unit);
    }
  }
  return cmdq;
}

void EventEffectList::RaiseEvent(event::OnCmdEvent type, Unit* unit, CmdAct* act) const {
  for (auto e : oncmd_elements_) {
    if (e->typeof(type)) {
      e->OnEvent(unit, act);
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

} // namespace core
} // namespace mengde

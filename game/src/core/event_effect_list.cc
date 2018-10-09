#include "event_effect_list.h"

#include "cmd.h"  // TODO Only for CmdQueue
#include "event_effect.h"
#include "util/common.h"

namespace mengde {
namespace core {

EventEffectList::EventEffectList() {}

EventEffectList::~EventEffectList() {
  for (auto e : general_elements_) {
    delete e;
  }
  for (auto e : oncmd_elements_) {
    delete e;
  }
}

void EventEffectList::AddGeneralEffect(GeneralEventEffect* e) { general_elements_.push_back(e); }

void EventEffectList::AddOnCmdEffect(OnCmdEventEffect* e) { oncmd_elements_.push_back(e); }

unique_ptr<Cmd> EventEffectList::RaiseEvent(event::GeneralEvent type, Unit* unit) const {
  CmdQueue* cmdq = new CmdQueue();
  for (auto e : general_elements_) {
    if (e->type(type)) {
      *cmdq += e->OnEvent(unit);
    }
  }
  return unique_ptr<Cmd>(cmdq);
}

void EventEffectList::RaiseEvent(event::OnCmdEvent type, Unit* unit, CmdAct* act) const {
  for (auto e : oncmd_elements_) {
    if (e->type(type)) {
      e->OnEvent(unit, act);
    }
  }
}

void EventEffectList::NextTurn() {
  // TODO Implement NextTurn
  /*
  elements_.erase(remove_if(elements_.begin(), elements_.end(), [] (EventEffectBase* e) {
    bool remove = (e->GetTurnsLeft() == 0);
    if (remove) delete e;
    return remove;
  }), elements_.end());

  for (auto e : elements_) {
    e->NextTurn();
  }
  */
}

void EventEffectList::iterate(const std::function<void(const EventEffectBase&)>& fn) const {
  for (auto e : general_elements_) {
    fn(*e);
  }
  for (auto e : oncmd_elements_) {
    fn(*e);
  }
}

}  // namespace core
}  // namespace mengde

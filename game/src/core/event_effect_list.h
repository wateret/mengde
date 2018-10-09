#ifndef MENGDE_CORE_EVENT_EFFECT_LIST_H_
#define MENGDE_CORE_EVENT_EFFECT_LIST_H_

#include <vector>

#include "event_effect.h"
#include "i_event.h"

namespace mengde {
namespace core {

class EventEffectList : public IEvent {
 public:
  virtual unique_ptr<Cmd> RaiseEvent(event::GeneralEvent, Unit *) const override;
  virtual void RaiseEvent(event::OnCmdEvent, Unit *, CmdAct *) const override;

 public:
  EventEffectList();
  ~EventEffectList();
  void AddGeneralEffect(GeneralEventEffect *);
  void AddOnCmdEffect(OnCmdEventEffect *);
  void NextTurn();
  void iterate(const std::function<void(const EventEffectBase &)> &fn) const;

 private:
  std::vector<GeneralEventEffect *> general_elements_;
  std::vector<OnCmdEventEffect *> oncmd_elements_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_EVENT_EFFECT_LIST_H_

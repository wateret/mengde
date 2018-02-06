#ifndef I_EVENT_H_
#define I_EVENT_H_

#include "event_types.h"

namespace mengde {
namespace core {

class Cmd;
class CmdAct;
class Unit;

// IEvent is an interface for raising game events, i.e. for notifying observers

class IEvent {
 public:
  virtual ~IEvent() = default;
  virtual void RaiseEvent(EventType, Unit*) = 0;
  virtual unique_ptr<Cmd> RaiseEvent(event::GeneralEvent, Unit*) const = 0;
  virtual void RaiseEvent(event::OnCmdEvent, Unit*, CmdAct*) const = 0;
};

} // namespace core
} // namespace mengde

#endif // I_EVENT_H_

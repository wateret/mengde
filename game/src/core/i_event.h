#ifndef MENGDE_CORE_I_EVENT_H_
#define MENGDE_CORE_I_EVENT_H_

#include <memory>

#include "event_types.h"

using std::unique_ptr;

namespace mengde {
namespace core {

class Cmd;
class CmdAct;
class Unit;

// IEvent is an interface for raising game events, i.e. for notifying observers

class IEvent {
 public:
  virtual ~IEvent() = default;
  virtual unique_ptr<Cmd> RaiseEvent(event::GeneralEvent, Unit*) const = 0;
  virtual void RaiseEvent(event::OnCmdEvent, Unit*, CmdAct*) const = 0;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_I_EVENT_H_

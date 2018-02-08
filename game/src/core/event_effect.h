#ifndef EVENT_EFFECT_H_
#define EVENT_EFFECT_H_

#include <limits>

#include "util/common.h"
#include "event_types.h"

namespace mengde {
namespace core {

class Cmd;
class CmdAct;
class Unit;

// EventEffectBase is an observer for game event handling
// There are two types of Events - GeneralEventEffect and OnCmdEventEffect.
// These two types both have the method `RaiseEvent` but note that their signatures are different.

class EventEffectBase {
 public:
  static const uint16_t kTurnInfinity = std::numeric_limits<uint16_t>::max();

 public:
  EventEffectBase(uint16_t turns_left);
  virtual ~EventEffectBase() = default;
  uint16_t GetTurnsLeft() { return turns_left_; }
  void NextTurn();

 private:
  uint16_t turns_left_;
};

// GeneralEventEffect is event effect that can generate Cmds

class GeneralEventEffect : public EventEffectBase {
 public:
  GeneralEventEffect(event::GeneralEvent type, uint16_t turns_left);
  bool typeof(event::GeneralEvent type) { return type_ == type; }
  virtual unique_ptr<Cmd> OnEvent(Unit*) = 0;

 private:
  event::GeneralEvent type_;
};

// OnCmdEventEffect is event effect that can modify CmdAction.
// Different from GeneralEventEffect, this cannot generate Cmds it can only modify CmdAction passed as an argument.

class OnCmdEventEffect : public EventEffectBase {
 public:
  OnCmdEventEffect(event::OnCmdEvent type, uint16_t turns_left);
  bool typeof(event::OnCmdEvent type) { return type_ == type; }
  virtual void OnEvent(Unit*, CmdAct*) = 0;

 private:
  event::OnCmdEvent type_;
};

// Children of GeneralEventEffect and OnCmdEventEffect
// The name of the class must be in the form of `{G|OC}EE.*` for the classes that inherit GeneralEventEffect or
// OnCmdEventEffect. G stands for General, OC for OnCmd.

class GEERestoreHp : public GeneralEventEffect {
 public:
  GEERestoreHp(event::GeneralEvent type, int ratio, int amount, uint16_t turns_left = kTurnInfinity);
  virtual unique_ptr<Cmd> OnEvent(Unit*);

 private:
  int ratio_;
  int adder_;
};

} // namespace core
} // namespace mengde

#endif // EVENT_EFFECT_H_

#ifndef MENGDE_CORE_EVENT_EFFECT_H_
#define MENGDE_CORE_EVENT_EFFECT_H_

#include <limits>

#include "event_types.h"
#include "util/common.h"

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
  virtual unique_ptr<Cmd> OnEvent(Unit* unit) = 0;
  bool typeof(event::GeneralEvent type) { return type_ == type; }

 private:
  event::GeneralEvent type_;
};

// OnCmdEventEffect is event effect that can modify CmdAction.
// Different from GeneralEventEffect, this cannot generate Cmds it can only modify CmdAction passed as an argument.

class OnCmdEventEffect : public EventEffectBase {
 public:
  OnCmdEventEffect(event::OnCmdEvent type, uint16_t turns_left);
  virtual void OnEvent(Unit* unit, CmdAct* act) = 0;
  bool typeof(event::OnCmdEvent type) { return type_ == type; }

 private:
  event::OnCmdEvent type_;
};

// Children of GeneralEventEffect and OnCmdEventEffect
// The name of the class must be in the form of `{G|OC}EE.*` for the classes that inherit GeneralEventEffect or
// OnCmdEventEffect. G stands for General, OC for OnCmd.

// GeneralEventEffect Dervatives

class GEERestoreHp : public GeneralEventEffect {
 public:
  GEERestoreHp(event::GeneralEvent type, int multiplier, int addend, uint16_t turns_left = kTurnInfinity);
  virtual unique_ptr<Cmd> OnEvent(Unit* unit) override;

 private:
  int multiplier_;
  int addend_;
};

// OnCmdEventEffect Dervatives

class OCEEPreemptiveAttack : public OnCmdEventEffect {
 public:
  OCEEPreemptiveAttack(event::OnCmdEvent type, uint16_t turns_left = kTurnInfinity);
  virtual void OnEvent(Unit* unit, CmdAct* act) override;
};

class OCEEEnhanceBasicAttack : public OnCmdEventEffect {
 public:
  OCEEEnhanceBasicAttack(event::OnCmdEvent type, int multiplier, int addend, uint16_t turns_left = kTurnInfinity);
  virtual void OnEvent(Unit* unit, CmdAct* act) override;

 private:
  int multiplier_;
  int addend_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_EVENT_EFFECT_H_

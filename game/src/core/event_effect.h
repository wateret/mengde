#ifndef MENGDE_CORE_EVENT_EFFECT_H_
#define MENGDE_CORE_EVENT_EFFECT_H_

#include "event_types.h"
#include "stat_modifier.h"
#include "turn_based.h"
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
  EventEffectBase(TurnBased turn = TurnBased{});
  virtual ~EventEffectBase() = default;
  const TurnBased& turn() const { return turn_; }
  void NextTurn();

 private:
  TurnBased turn_;
};

// GeneralEventEffect is event effect that can generate Cmds

class GeneralEventEffect : public EventEffectBase {
 public:
  GeneralEventEffect(event::GeneralEvent type, TurnBased turn = TurnBased{});
  virtual unique_ptr<Cmd> OnEvent(Unit* unit) = 0;
  bool type(event::GeneralEvent type) { return type_ == type; }
  event::GeneralEvent type() const { return type_; }

 private:
  event::GeneralEvent type_;
};

// OnCmdEventEffect is event effect that can modify CmdAction.
// Different from GeneralEventEffect, this cannot generate Cmds it can only modify CmdAction passed as an argument.

class OnCmdEventEffect : public EventEffectBase {
 public:
  OnCmdEventEffect(event::OnCmdEvent type, TurnBased turn = TurnBased{});
  virtual void OnEvent(Unit* unit, CmdAct* act) = 0;
  bool type(event::OnCmdEvent type) { return type_ == type; }
  event::OnCmdEvent type() const { return type_; }

 private:
  event::OnCmdEvent type_;
};

// Children of GeneralEventEffect and OnCmdEventEffect
// The name of the class must be in the form of `{G|OC}EE.*` for the classes that inherit GeneralEventEffect or
// OnCmdEventEffect. G stands for General, OC for OnCmd.

// GeneralEventEffect Dervatives

class GEERestoreHp : public GeneralEventEffect {
 public:
  GEERestoreHp(event::GeneralEvent type, int multiplier, int addend, TurnBased turn = TurnBased{});
  virtual unique_ptr<Cmd> OnEvent(Unit* unit) override;
  AttributeChange stat_mod() const { return {static_cast<int16_t>(addend_), static_cast<int16_t>(multiplier_)}; }

 private:
  int multiplier_;
  int addend_;
};

// OnCmdEventEffect Dervatives

class OCEEPreemptiveAttack : public OnCmdEventEffect {
 public:
  OCEEPreemptiveAttack(event::OnCmdEvent type, TurnBased turn = TurnBased{});
  virtual void OnEvent(Unit* unit, CmdAct* act) override;
};

class OCEEEnhanceBasicAttack : public OnCmdEventEffect {
 public:
  OCEEEnhanceBasicAttack(event::OnCmdEvent type, int multiplier, int addend, TurnBased turn = TurnBased{});
  virtual void OnEvent(Unit* unit, CmdAct* act) override;
  AttributeChange stat_mod() const { return {static_cast<int16_t>(addend_), static_cast<int16_t>(multiplier_)}; }

 private:
  int multiplier_;
  int addend_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_EVENT_EFFECT_H_

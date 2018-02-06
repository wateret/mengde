#ifndef EQUIPMENT_H_
#define EQUIPMENT_H_

#include <string>

#include "event_effect_list.h"
#include "stat_modifier_list.h"
#include "util/common.h"
#include "i_event.h"

namespace mengde {
namespace core {

class EventEffect;
class StatModifier;

class Equipment : public IEvent {
 public:
  enum class Type {
    kNone,
    kWeapon,
    kArmor,
    kAid
  };

 public:
  void RaiseEvent(EventType, Unit*) override;
  virtual unique_ptr<Cmd> RaiseEvent(event::GeneralEvent, Unit*) const override;
  virtual void RaiseEvent(event::OnCmdEvent, Unit*, CmdAct*) const override;

 public:
  Equipment(const std::string&, Type);
  string GetId() const { return id_; }
  Type GetType() const { return type_; }
  void AddModifier(StatModifier*);
  void AddEffect(EventEffect*);
  Attribute CalcAddends() const;
  Attribute CalcMultipliers() const;

 private:
  string id_;
  Type type_;
  StatModifierList modifier_list_;
  EventEffectList  effect_list_;
};

} // namespace core
} // namespace mengde

#endif // EQUIPMENT_H_

#ifndef MENGDE_CORE_EQUIPMENT_H_
#define MENGDE_CORE_EQUIPMENT_H_

#include <string>

#include "i_event.h"
#include "util/common.h"
#include "volatile_attribute.h"

namespace mengde {
namespace core {

class EventEffect;
class AttributeModifier;

class Equipment : public IEvent {
 public:
  enum class Type { kNone, kWeapon, kArmor, kAid };

 public:
  virtual unique_ptr<Cmd> RaiseEvent(event::GeneralEvent, Unit*) const override;
  virtual void RaiseEvent(event::OnCmdEvent, Unit*, CmdAct*) const override;

 public:
  Equipment(const std::string&, Type, unique_ptr<VolatileAttribute>&& va = std::make_unique<VolatileAttribute>());
  const string& GetId() const { return id_; }
  Type GetType() const { return type_; }
  void AddModifier(AttributeModifier*);
  void AddEffect(EventEffect*);
  void AddGeneralEffect(GeneralEventEffect*);
  void AddOnCmdEffect(OnCmdEventEffect*);
  Attribute CalcAddends() const;
  Attribute CalcMultipliers() const;
  const VolatileAttribute& volatile_attribute() const { return *volatile_attribute_; }

 private:
  string id_;
  Type type_;
  unique_ptr<VolatileAttribute> volatile_attribute_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_EQUIPMENT_H_

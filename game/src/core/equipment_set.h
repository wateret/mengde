#ifndef MENGDE_CORE_EQUIPMENT_SET_H_
#define MENGDE_CORE_EQUIPMENT_SET_H_

#include "equipment_slot.h"
#include "i_event.h"
#include "stat.h"

namespace mengde {
namespace core {

class IEquipper;

class EquipmentSet : public IEvent {
 public:
  EquipmentSet(IEquipper*);
  void             CopyEquipmentSet(const EquipmentSet&);
  EquipmentSet*    Clone(IEquipper*) const;
  void             SetEquipment(const Equipment*);
  void             SetWeapon(const Equipment*);
  void             SetArmor(const Equipment*);
  void             SetAid(const Equipment*);
  const Equipment* GetEquipment(Equipment::Type) const;
  const Equipment* GetWeapon() const;
  const Equipment* GetArmor() const;
  const Equipment* GetAid() const;
  Attribute        CalcAddends() const;
  Attribute        CalcMultipliers() const;

 public:
  virtual unique_ptr<Cmd> RaiseEvent(event::GeneralEvent, Unit*) const override;
  virtual void            RaiseEvent(event::OnCmdEvent, Unit*, CmdAct*) const override;

 private:
  IEquipper*    equipper_;
  EquipmentSlot slot_weapon_;
  EquipmentSlot slot_armor_;
  EquipmentSlot slot_aid_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_EQUIPMENT_SET_H_

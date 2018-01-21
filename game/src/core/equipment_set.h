#ifndef EQUIPMENT_SET_H_
#define EQUIPMENT_SET_H_

#include "stat.h"
#include "equipment_slot.h"

namespace mengde {
namespace core {

class IEquipper;

class EquipmentSet {
 public:
  EquipmentSet(IEquipper*);
  void CopyEquipmentSet(const EquipmentSet&);
  EquipmentSet* Clone(IEquipper*) const;
  void SetEquipment(const Equipment*);
  void SetWeapon(const Equipment*);
  void SetArmor(const Equipment*);
  void SetAid(const Equipment*);
  const Equipment* GetEquipment(Equipment::Type) const;
  const Equipment* GetWeapon() const;
  const Equipment* GetArmor() const;
  const Equipment* GetAid() const;
  Attribute CalcAddends() const;
  Attribute CalcMultipliers() const;

 private:
  IEquipper* equipper_;
  EquipmentSlot slot_weapon_;
  EquipmentSlot slot_armor_;
  EquipmentSlot slot_aid_;
};

} // namespace core
} // namespace mengde

#endif // EQUIPMENT_SET_H_

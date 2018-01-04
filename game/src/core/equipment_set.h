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
  void SetWeapon(Equipment*);
  void SetArmor(Equipment*);
  void SetAid(Equipment*);
  const Equipment* GetWeapon() const;
  const Equipment* GetArmor() const;
  const Equipment* GetAid() const;
  Stat CalcModifierAddends() const;
  Stat CalcModifierMultipliers() const;

 private:
  // NOTE We have equipper_ as a raw pointer but the some actual instances(Hero) is always treated with shared_ptr
  //      We aware that shared_ptr<Hero> is live at least until its corresponding Unit is deleted so it is OK with this way.
  //      Also we would not want to use enable_shared_from_this along with weak_ptr since it would be too much for this.
  IEquipper* equipper_;
  EquipmentSlot slot_weapon_;
  EquipmentSlot slot_armor_;
  EquipmentSlot slot_aid_;
};

} // namespace core
} // namespace mengde

#endif // EQUIPMENT_SET_H_

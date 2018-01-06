#include "equipment_set.h"

#include "i_equipper.h"

namespace mengde {
namespace core {

EquipmentSet::EquipmentSet(IEquipper* equipper) : equipper_(equipper),
    slot_weapon_(Equipment::Type::kWeapon), slot_armor_(Equipment::Type::kArmor), slot_aid_(Equipment::Type::kAid) {
}

void EquipmentSet::SetEquipment(const Equipment* e) {
  switch (e->GetType()) {
    case Equipment::Type::kWeapon:
      SetWeapon(e);
      break;
    case Equipment::Type::kArmor:
      SetArmor(e);
      break;
    case Equipment::Type::kAid:
      SetAid(e);
      break;
    default:
      UNREACHABLE("Unknown Equipment Type");
      break;
  }
}

void EquipmentSet::SetWeapon(const Equipment* e) {
  slot_weapon_.PutEquipmentOn(e);
  equipper_->UpdateStat();
}

void EquipmentSet::SetArmor(const Equipment* e) {
  slot_armor_.PutEquipmentOn(e);
  equipper_->UpdateStat();
}

void EquipmentSet::SetAid(const Equipment* e) {
  slot_aid_.PutEquipmentOn(e);
  equipper_->UpdateStat();
}

const Equipment* EquipmentSet::GetEquipment(Equipment::Type type) const {
  switch (type) {
    case Equipment::Type::kWeapon:
      return GetWeapon();
    case Equipment::Type::kArmor:
      return GetArmor();
    case Equipment::Type::kAid:
      return GetAid();
    default:
      UNREACHABLE("Unknown Equipment Type");
      break;
  }
}

const Equipment* EquipmentSet::GetWeapon() const {
  return slot_weapon_.GetEquipment();
}

const Equipment* EquipmentSet::GetArmor() const {
  return slot_armor_.GetEquipment();
}

const Equipment* EquipmentSet::GetAid() const {
  return slot_aid_.GetEquipment();
}

Stat EquipmentSet::CalcModifierAddends() const {
  return slot_weapon_.CalcModifierAddends() + slot_weapon_.CalcModifierAddends() + slot_weapon_.CalcModifierAddends();
}

Stat EquipmentSet::CalcModifierMultipliers() const {
  return slot_weapon_.CalcModifierMultipliers() + slot_weapon_.CalcModifierMultipliers() + slot_weapon_.CalcModifierMultipliers();
}

} // namespace core
} // namespace mengde

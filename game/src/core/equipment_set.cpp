#include "equipment_set.h"
#include "i_equipper.h"

EquipmentSet::EquipmentSet(IEquipper* equipper) : equipper_(equipper),
    slot_weapon_(Equipment::Type::kWeapon), slot_armor_(Equipment::Type::kArmor), slot_aid_(Equipment::Type::kAid) {
}

void EquipmentSet::SetWeapon(Equipment* e) {
  slot_weapon_.PutEquipmentOn(e);
  equipper_->UpdateStat();
}

void EquipmentSet::SetArmor(Equipment* e) {
  slot_armor_.PutEquipmentOn(e);
  equipper_->UpdateStat();
}

void EquipmentSet::SetAid(Equipment* e) {
  slot_aid_.PutEquipmentOn(e);
  equipper_->UpdateStat();
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

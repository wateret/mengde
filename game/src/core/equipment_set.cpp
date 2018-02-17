#include "equipment_set.h"

#include "cmd.h"  // TODO included for CmdQueue only
#include "i_equipper.h"

namespace mengde {
namespace core {

EquipmentSet::EquipmentSet(IEquipper* equipper)
    : equipper_(equipper),
      slot_weapon_(Equipment::Type::kWeapon),
      slot_armor_(Equipment::Type::kArmor),
      slot_aid_(Equipment::Type::kAid) {}

void EquipmentSet::CopyEquipmentSet(const EquipmentSet& eqset) {
  SetWeapon(eqset.GetWeapon());
  SetArmor(eqset.GetArmor());
  SetAid(eqset.GetAid());
}

// Clones this object with given Equipper*
// returns newly created object
EquipmentSet* EquipmentSet::Clone(IEquipper* equipper) const {
  EquipmentSet* cloned = new EquipmentSet(equipper);
  cloned->slot_weapon_ = this->slot_weapon_;
  cloned->slot_armor_  = this->slot_armor_;
  cloned->slot_aid_    = this->slot_aid_;
  /*
  cloned->PutEquipmentOn(this->GetWeapon());
  cloned->PutEquipmentOn(this->GetArmor());
  cloned->PutEquipmentOn(this->GetVid());
  */
  return cloned;
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
      return nullptr;
  }
}

const Equipment* EquipmentSet::GetWeapon() const { return slot_weapon_.GetEquipment(); }

const Equipment* EquipmentSet::GetArmor() const { return slot_armor_.GetEquipment(); }

const Equipment* EquipmentSet::GetAid() const { return slot_aid_.GetEquipment(); }

Attribute EquipmentSet::CalcAddends() const {
  return slot_weapon_.CalcAddends() + slot_armor_.CalcAddends() + slot_aid_.CalcAddends();
}

Attribute EquipmentSet::CalcMultipliers() const {
  return slot_weapon_.CalcMultipliers() + slot_armor_.CalcMultipliers() + slot_aid_.CalcMultipliers();
}

unique_ptr<Cmd> EquipmentSet::RaiseEvent(event::GeneralEvent type, Unit* unit) const {
  auto             cmdq   = unique_ptr<CmdQueue>(new CmdQueue());
  const Equipment* weapon = GetWeapon();
  const Equipment* armor  = GetArmor();
  const Equipment* aid    = GetAid();
  if (weapon != nullptr) cmdq->Append(weapon->RaiseEvent(type, unit));
  if (armor != nullptr) cmdq->Append(armor->RaiseEvent(type, unit));
  if (aid != nullptr) cmdq->Append(aid->RaiseEvent(type, unit));
  return cmdq;
}

void EquipmentSet::RaiseEvent(event::OnCmdEvent type, Unit* unit, CmdAct* act) const {
  const Equipment* weapon = GetWeapon();
  const Equipment* armor  = GetArmor();
  const Equipment* aid    = GetAid();
  if (weapon != nullptr) weapon->RaiseEvent(type, unit, act);
  if (armor != nullptr) armor->RaiseEvent(type, unit, act);
  if (aid != nullptr) aid->RaiseEvent(type, unit, act);
}

}  // namespace core
}  // namespace mengde

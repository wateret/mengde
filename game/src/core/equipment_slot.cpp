#include "equipment_slot.h"
#include "util/common.h"

EquipmentSlot::EquipmentSlot(Equipment::Type type)
    : type_(type), equipment_(NULL) {
}

Equipment* EquipmentSlot::PutEquipmentOn(Equipment* equipment) {
  if (equipment->GetType() == type_) {
    Equipment* cur_equipment = equipment_;
    equipment_ = equipment;
    return cur_equipment;
  } else {
    LOG_WARNING("Invalid type of equipment for the slot. The equipment is not put on");
    return nullptr;
  }
}

Stat EquipmentSlot::CalcModifierAddends() {
  if (IsEquipmentOn()) {
    return equipment_->CalcModifierAddends();
  } else {
    return {0, 0, 0, 0, 0};
  }
}

Stat EquipmentSlot::CalcModifierMultipliers() {
  if (IsEquipmentOn()) {
    return equipment_->CalcModifierMultipliers();
  } else {
    return {0, 0, 0, 0, 0};
  }
}

#include "equipment_slot.h"

#include "util/common.h"

namespace mengde {
namespace core {

EquipmentSlot::EquipmentSlot(Equipment::Type type)
    : type_(type), equipment_(NULL) {
}

const Equipment* EquipmentSlot::PutEquipmentOn(const Equipment* equipment) {
  if (equipment == nullptr || equipment->GetType() == type_) {
    const Equipment* cur_equipment = equipment_;
    equipment_ = equipment;
    return cur_equipment;
  } else {
    LOG_WARNING("Invalid type of equipment for the slot. The equipment is not put on");
    return nullptr;
  }
}

Stat EquipmentSlot::CalcModifierAddends() const {
  if (IsEquipmentOn()) {
    return equipment_->CalcModifierAddends();
  } else {
    return {0, 0, 0, 0, 0};
  }
}

Stat EquipmentSlot::CalcModifierMultipliers() const {
  if (IsEquipmentOn()) {
    return equipment_->CalcModifierMultipliers();
  } else {
    return {0, 0, 0, 0, 0};
  }
}

} // namespace core
} // namespace mengde

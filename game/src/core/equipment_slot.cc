#include "equipment_slot.h"

#include "util/common.h"

namespace mengde {
namespace core {

EquipmentSlot::EquipmentSlot(Equipment::Type type) : type_(type), equipment_(NULL) {}

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

Attribute EquipmentSlot::CalcAddends() const {
  if (IsEquipmentOn()) {
    return equipment_->CalcAddends();
  } else {
    return {0, 0, 0, 0, 0};
  }
}

Attribute EquipmentSlot::CalcMultipliers() const {
  if (IsEquipmentOn()) {
    return equipment_->CalcMultipliers();
  } else {
    return {0, 0, 0, 0, 0};
  }
}

}  // namespace core
}  // namespace mengde

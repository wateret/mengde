#ifndef MENGDE_CORE_EQUIPMENT_SLOT_H_
#define MENGDE_CORE_EQUIPMENT_SLOT_H_

#include "equipment.h"
#include "stat.h"

namespace mengde {
namespace core {

class EquipmentSlot {
 public:
  EquipmentSlot(Equipment::Type);
  const Equipment* PutEquipmentOn(const Equipment*);
  Attribute        CalcAddends() const;
  Attribute        CalcMultipliers() const;
  bool             IsEquipmentOn() const { return equipment_ != NULL; }
  const Equipment* GetEquipment() const { return equipment_; }

 private:
  Equipment::Type  type_;
  const Equipment* equipment_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_EQUIPMENT_SLOT_H_

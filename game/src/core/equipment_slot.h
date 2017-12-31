#ifndef EQUIPMENT_SLOT_H_
#define EQUIPMENT_SLOT_H_

#include "equipment.h"
#include "stat.h"

class EquipmentSlot {
 public:
  EquipmentSlot(Equipment::Type);
  const Equipment* PutEquipmentOn(const Equipment*);
  Stat CalcModifierAddends() const;
  Stat CalcModifierMultipliers() const;
  bool IsEquipmentOn() const { return equipment_ != NULL; }
  const Equipment* GetEquipment() const { return equipment_; }

 private:
  Equipment::Type type_;
  const Equipment* equipment_;
};

#endif // EQUIPMENT_SLOT_H_

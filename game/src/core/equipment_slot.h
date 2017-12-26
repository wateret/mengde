#ifndef EQUIPMENT_SLOT_H_
#define EQUIPMENT_SLOT_H_

#include "equipment.h"
#include "stat.h"

class EquipmentSlot {
 public:
  EquipmentSlot(Equipment::Type);
  Equipment* PutEquipmentOn(Equipment*);
  Stat CalcModifierAddends();
  Stat CalcModifierMultipliers();
  bool IsEquipmentOn() { return equipment_ != NULL; }
  Equipment* GetEquipment() { return equipment_; }

 private:
  Equipment::Type type_;
  Equipment* equipment_;
};

#endif // EQUIPMENT_SLOT_H_

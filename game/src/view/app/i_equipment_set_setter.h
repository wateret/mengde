#ifndef I_EQUIPMENT_SET_SETTER_H_
#define I_EQUIPMENT_SET_SETTER_H_

#include "util/common.h"

class EquipmentSet;

class IEquipmentSetSetter {
 public:
  virtual void SetEquipmentSet(EquipmentSet*) = 0;
};

#endif // I_EQUIPMENT_SET_SETTER_H_

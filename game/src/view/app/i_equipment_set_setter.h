#ifndef I_EQUIPMENT_SET_SETTER_H_
#define I_EQUIPMENT_SET_SETTER_H_

#include "util/common.h"

namespace mengde {
namespace core {
  class EquipmentSet;
}
}

class IEquipmentSetSetter {
 public:
  virtual void SetEquipmentSet(mengde::core::EquipmentSet*) = 0;
};

#endif // I_EQUIPMENT_SET_SETTER_H_

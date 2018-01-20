#ifndef I_EQUIPMENT_SET_SETTER_H_
#define I_EQUIPMENT_SET_SETTER_H_

#include "util/common.h"

namespace mengde {
namespace gui {
namespace app {

namespace mengde {
namespace core {
  class EquipmentSet;
}
}

class IEquipmentSetSetter {
 public:
  virtual void SetEquipmentSet(const mengde::core::EquipmentSet*) = 0;
};

} // namespace app
} // namespace gui
} // namespace mengde

#endif // I_EQUIPMENT_SET_SETTER_H_

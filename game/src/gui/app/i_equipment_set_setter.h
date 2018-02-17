#ifndef I_EQUIPMENT_SET_SETTER_H_
#define I_EQUIPMENT_SET_SETTER_H_

#include "util/common.h"

#include "common.h"

namespace mengde {
namespace core {
class EquipmentSet;
}
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class IEquipmentSetSetter {
 public:
  virtual void SetEquipmentSet(const core::EquipmentSet*) = 0;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // I_EQUIPMENT_SET_SETTER_H_

#ifndef STAGE_UNIT_MANAGER_H_
#define STAGE_UNIT_MANAGER_H_

#include "util/common.h"

namespace mengde {
namespace core {

class Unit;

//
// StageUnitManager manages units for a Stage
// All units are actually freed here meanwhile allocation is from other classes.
//

class StageUnitManager {
 public:
  StageUnitManager();
  uint32_t Deploy(Unit*);
  void     Kill(Unit*);
  Unit*    Get(uint32_t);
  void     ForEach(function<void(Unit*)>);

 private:
  vector<Unit*> units_;
  //  vector<bool>  units_status_;
};

}  // namespace core
}  // namespace mengde

#endif  // STAGE_UNIT_MANAGER_H_

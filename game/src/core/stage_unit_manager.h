#ifndef MENGDE_CORE_STAGE_UNIT_MANAGER_H_
#define MENGDE_CORE_STAGE_UNIT_MANAGER_H_

#include "util/common.h"
#include "id.h"

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
  UId Deploy(Unit*);
  void Kill(Unit*);
  Unit* Get(const UId& id);
  void ForEach(function<void(Unit*)>);
  void ForEachIdxConst(function<void(uint32_t, const Unit*)> fn) const;

 private:
  vector<Unit*> units_;
  //  vector<bool>  units_status_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_STAGE_UNIT_MANAGER_H_

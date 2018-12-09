#ifndef MENGDE_CORE_STAGE_UNIT_MANAGER_H_
#define MENGDE_CORE_STAGE_UNIT_MANAGER_H_

#include <unordered_map>

#include "ai_mode.h"
#include "id.h"
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
  UId Deploy(Unit*);
  void Kill(Unit*);
  Unit* Get(const UId& id);
  void SetAIMode(const UId& id, AIMode mode);
  void ForEach(function<void(Unit*)>);
  void ForEachConst(function<void(const Unit*)> fn) const;

 private:
  vector<Unit*> units_;
  std::unordered_map<UId, AIMode> ai_modes_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_STAGE_UNIT_MANAGER_H_

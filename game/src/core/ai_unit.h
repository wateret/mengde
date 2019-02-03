#ifndef MENGDE_CORE_AI_UNIT_H_
#define MENGDE_CORE_AI_UNIT_H_

#include "user_interface.h"

namespace mengde {
namespace core {

class IAIUnit {
 public:
  virtual ~IAIUnit() = default;
  virtual void play(const UnitKey& unit_key, UserInterface* ui) const = 0;
};

class AIUnitNone : public IAIUnit {
 public:
  virtual void play(const UnitKey& unit_key, UserInterface* ui) const override;
};

class AIUnitDoNothing : public IAIUnit {
 public:
  virtual void play(const UnitKey& unit_key, UserInterface* ui) const override;
};

class AIUnitUnitInRangeRandom : public IAIUnit {
 public:
  virtual void play(const UnitKey& unit_key, UserInterface* ui) const override;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_AI_UNIT_H_

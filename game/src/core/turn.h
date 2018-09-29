#ifndef MENGDE_CORE_TURN_H_
#define MENGDE_CORE_TURN_H_

#include "unit.h"

namespace mengde {
namespace core {

class Turn {
 public:
  Turn(uint16_t limit, uint16_t current = 1);
  bool Next();
  uint16_t current() const { return current_; }
  uint16_t limit() const { return limit_; }
  Force force() const { return force_; }

 private:
  uint16_t current_;
  uint16_t limit_;
  Force force_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_TURN_H_

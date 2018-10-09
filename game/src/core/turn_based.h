#ifndef MENGDE_CORE_TURN_BASED_H_
#define MENGDE_CORE_TURN_BASED_H_

#include <stdint.h>
#include <limits>

namespace mengde {
namespace core {

class TurnBased {
 public:
  static const uint16_t kInfinity = std::numeric_limits<uint16_t>::max();

 public:
  TurnBased(uint16_t turns = kInfinity);
  uint16_t left() const { return turns_left_; }
  void Next();

 private:
  uint16_t turns_left_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_TURN_BASED_H_

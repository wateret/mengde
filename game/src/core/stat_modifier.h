#ifndef MENGDE_CORE_STAT_MODIFIER_H_
#define MENGDE_CORE_STAT_MODIFIER_H_

#include <limits>
#include <string>
#include "util/common.h"

namespace mengde {
namespace core {

class StatModifier {
 public:
  static const uint16_t kTurnInfinity = std::numeric_limits<uint16_t>::max();

 public:
  StatModifier(const std::string&, uint16_t stat_id, uint16_t addend, uint16_t multiplier,
               uint16_t turns_left = kTurnInfinity);
  std::string GetId() const { return id_; }
  uint16_t GetStatId() const { return stat_id_; }
  int16_t GetAddend() const { return addend_; }
  uint16_t GetMultiplier() const { return multiplier_; }
  uint16_t GetTurnsLeft() const { return turns_left_; }
  void NextTurn();

 private:
  std::string id_;
  uint16_t stat_id_;
  int16_t addend_;
  uint16_t multiplier_;
  uint16_t turns_left_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_STAT_MODIFIER_H_

#ifndef MENGDE_CORE_STAT_MODIFIER_H_
#define MENGDE_CORE_STAT_MODIFIER_H_

#include <limits>
#include <string>
#include "util/common.h"

namespace mengde {
namespace core {

struct StatMod {
  int16_t addend;
  int16_t multiplier;
};

class StatModifier {
 public:
  static const uint16_t kTurnInfinity = std::numeric_limits<uint16_t>::max();

 public:
  StatModifier(const std::string& id, uint16_t stat_id, StatMod mod, uint16_t turns_left = kTurnInfinity);
  std::string id() const { return id_; }
  uint16_t stat_id() const { return stat_id_; }
  int16_t addend() const { return mod_.addend; }
  uint16_t multiplier() const { return mod_.multiplier; }
  uint16_t turns_left() const { return turns_left_; }
  void NextTurn();

 public:
  string ToString() const;

 private:
  std::string id_;
  uint16_t stat_id_;
  uint16_t turns_left_;
  StatMod mod_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_STAT_MODIFIER_H_

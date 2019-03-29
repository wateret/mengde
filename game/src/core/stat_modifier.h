#ifndef MENGDE_CORE_STAT_MODIFIER_H_
#define MENGDE_CORE_STAT_MODIFIER_H_

#include <string>
#include "turn_based.h"
#include "util/common.h"

namespace mengde {
namespace core {

struct StatMod {
  int16_t addend;
  int16_t multiplier;
};

class StatModifier {
 public:
  StatModifier(const std::string& id, uint16_t stat_id, StatMod mod, TurnBased turn = TurnBased{});
  std::string id() const { return id_; }
  uint16_t stat_id() const { return stat_id_; }
  int16_t addend() const { return mod_.addend; }
  uint16_t multiplier() const { return mod_.multiplier; }
  const StatMod& mod() const { return mod_; }
  const TurnBased& turn() const { return turn_; }
  void NextTurn();

 public:
  string ToString() const;

 private:
  std::string id_;
  uint16_t stat_id_;
  TurnBased turn_;
  StatMod mod_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_STAT_MODIFIER_H_

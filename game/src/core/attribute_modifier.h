#ifndef MENGDE_CORE_ATTRIBUTE_MODIFIER_H_
#define MENGDE_CORE_ATTRIBUTE_MODIFIER_H_

#include <string>
#include "turn_based.h"
#include "util/common.h"

namespace mengde {
namespace core {

struct AttributeChange {
  int16_t addend;
  int16_t multiplier;
};

class AttributeModifier {
 public:
  AttributeModifier(const std::string& id, uint16_t stat_id, AttributeChange mod, TurnBased turn = TurnBased{});
  std::string id() const { return id_; }
  uint16_t stat_id() const { return stat_id_; }
  int16_t addend() const { return change_.addend; }
  uint16_t multiplier() const { return change_.multiplier; }
  const AttributeChange& change() const { return change_; }
  const TurnBased& turn() const { return turn_; }
  void NextTurn();

 public:
  string ToString() const;

 private:
  std::string id_;
  uint16_t stat_id_;
  TurnBased turn_;
  AttributeChange change_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_ATTRIBUTE_MODIFIER_H_

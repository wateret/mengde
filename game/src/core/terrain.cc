#include "terrain.h"

#include "util/common.h"

namespace mengde {
namespace core {

Terrain::Terrain(const std::string& id, char ascii, const std::vector<int>& move_costs, const std::vector<int>& effects)
    : id_(id), ascii_{ascii}, move_costs_(move_costs), effects_(effects) {}

int Terrain::GetMoveCost(int class_idx) const {
  ASSERT(class_idx < (int)move_costs_.size());
  return move_costs_[class_idx];
}

int Terrain::GetEffect(int class_idx) const {
  ASSERT(class_idx < (int)move_costs_.size());
  return effects_[class_idx];
}

}  // namespace core
}  // namespace mengde

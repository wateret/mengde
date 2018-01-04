#include "terrain.h"

#include "util/common.h"

namespace mengde {
namespace core {

Terrain::Terrain(const std::string& name,
                 const std::vector<int>& move_costs,
                 const std::vector<int>& effects)
    : index_(-1), name_(name), move_costs_(move_costs), effects_(effects) {
}

int Terrain::GetIndex() {
  ASSERT(index_ != -1);
  return index_;
}

void Terrain::SetIndex(int i) {
  ASSERT(index_ == -1);
  index_ = i;
}

int Terrain::GetMoveCost(int class_idx) {
  ASSERT(class_idx < (int)move_costs_.size());
  return move_costs_[class_idx];
}

int Terrain::GetEffect(int class_idx) {
  ASSERT(class_idx < (int)move_costs_.size());
  return effects_[class_idx];
}

} // namespace core
} // namespace mengde

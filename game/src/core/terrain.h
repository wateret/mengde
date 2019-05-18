#ifndef MENGDE_CORE_TERRAIN_H_
#define MENGDE_CORE_TERRAIN_H_

#include <string>
#include <vector>

namespace mengde {
namespace core {

class Terrain {
 public:
  Terrain(const std::string&, const std::vector<int>&, const std::vector<int>&);
  const std::string& id() const { return id_; }
  int GetMoveCost(int) const;
  int GetEffect(int) const;
  const std::vector<int>& move_costs() const { return move_costs_; }
  const std::vector<int>& class_effects() const { return effects_; }

 private:
  std::string id_;
  std::vector<int> move_costs_;
  std::vector<int> effects_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_TERRAIN_H_

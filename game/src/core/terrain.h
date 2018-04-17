#ifndef MENGDE_CORE_TERRAIN_H_
#define MENGDE_CORE_TERRAIN_H_

#include <string>
#include <vector>

namespace mengde {
namespace core {

class Terrain {
 public:
  Terrain(const std::string&, const std::vector<int>&, const std::vector<int>&);
  int         GetIndex();
  void        SetIndex(int);
  std::string GetName() { return name_; }
  int         GetMoveCost(int);
  int         GetEffect(int);

 private:
  int              index_;
  std::string      name_;
  std::vector<int> move_costs_;
  std::vector<int> effects_;
};

}  // namespace core
}  // namespace mengde

#endif // MENGDE_CORE_TERRAIN_H_

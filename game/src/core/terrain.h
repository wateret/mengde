#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include <vector>

class Terrain {
 public:
  Terrain(const std::string&, const std::vector<int>&, const std::vector<int>&);
  int GetIndex();
  void SetIndex(int);
  std::string GetName() { return name_; }
  int GetMoveCost(int);
  int GetEffect(int);

 private:
  int index_;
  std::string name_;
  std::vector<int> move_costs_;
  std::vector<int> effects_;
};

#endif /* TERRAIN_H */

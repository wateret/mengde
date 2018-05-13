#ifndef MENGDE_CORE_DEPLOYER_H_
#define MENGDE_CORE_DEPLOYER_H_

#include <set>
#include <unordered_set>
#include <utility>

#include "util/common.h"

namespace mengde {
namespace core {

class Hero;

struct DeployElement {
  const Hero* hero;
  uint32_t no;

  DeployElement() : hero(nullptr), no(0) {}
  DeployElement(const Hero* hero, uint32_t no) : hero(hero), no(no) {}
};

struct DeployElementHash {
  size_t operator()(const mengde::core::DeployElement& o) const noexcept {
    return std::hash<const mengde::core::Hero*>{}(o.hero);
  }
};

struct DeployerComparer {
  bool operator()(const DeployElement& lhs, const DeployElement& rhs) const;
};

}  // namespace core
}  // namespace mengde

namespace mengde {
namespace core {

struct DeployInfoUnselectable {
  Vec2D position;
  const Hero* hero;

  DeployInfoUnselectable(Vec2D position, const Hero* hero) : position(position), hero(hero) {}
};

struct DeployInfoSelectable {
  Vec2D position;

  DeployInfoSelectable(Vec2D position) : position(position) {}
};

class Deployer {
 private:
  enum class Type { kNone, kUnselectable, kSelectable };

 public:
  typedef std::unordered_set<DeployElement, DeployElementHash, DeployerComparer> AssignmentContainer;
  typedef function<void(const DeployElement&)> ForEachFn;

 public:
  Deployer(const vector<DeployInfoUnselectable>&, const vector<DeployInfoSelectable>&, uint32_t);
  uint32_t Assign(const Hero* hero);
  uint32_t Unassign(const Hero* hero);
  uint32_t Find(const Hero* hero);
  uint32_t GetNextSelectableNo();
  void ForEach(ForEachFn);
  Vec2D GetPosition(const Hero* hero);
  bool IsReady();
  uint32_t GetNumAssigned();

 private:
  std::pair<Type, DeployElement> FindImpl(const Hero* hero);
  static vector<DeployElement> AsOrderedVector(AssignmentContainer&);

 private:
  vector<DeployInfoUnselectable> unselectable_info_list_;
  vector<DeployInfoSelectable> selectable_info_list_;
  uint32_t num_required_;
  AssignmentContainer unselectable_assignment_;
  AssignmentContainer selectable_assignment_;
  std::set<uint32_t> available_no_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_DEPLOYER_H_

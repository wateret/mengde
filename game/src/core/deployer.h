#ifndef DEPLOYER_H_
#define DEPLOYER_H_

#include <unordered_set>
#include <set>
#include <utility>

#include "util/common.h"

namespace mengde {
namespace core {

class Hero;

struct DeployElement {
  shared_ptr<const Hero> hero;
  uint32_t no;

  DeployElement(const shared_ptr<const Hero>& hero, uint32_t no) : hero(hero), no(no) {}
};

struct DeployerComparer {
  bool operator()(const DeployElement& lhs, const DeployElement& rhs);
};

} // namespace core
} // namespace mengde

namespace std
{
  template<>
  struct hash<mengde::core::DeployElement>
  {
    size_t operator()(mengde::core::DeployElement const& o) const noexcept
    {
      return hash<shared_ptr<const mengde::core::Hero>>{}(o.hero);
    }
  };
}

namespace mengde {
namespace core {

struct DeployInfoUnselectable {
  Vec2D position;
  shared_ptr<const Hero> hero;

  DeployInfoUnselectable(Vec2D position, const shared_ptr<const Hero>& hero) : position(position), hero(hero) {}
};

struct DeployInfoSelectable {
  Vec2D position;

  DeployInfoSelectable(Vec2D position) : position(position) {}
};

class Deployer {
 private:
  enum class Type {
    kNone,
    kUnselectable,
    kSelectable
  };

 public:
  typedef std::unordered_set<DeployElement, std::hash<DeployElement>, DeployerComparer> AssignmentContainer;
  typedef function<void(const DeployElement&)> ForEachFn;

 public:
  Deployer(const vector<DeployInfoUnselectable>&, const vector<DeployInfoSelectable>&, uint32_t);
  uint32_t Assign(const shared_ptr<const Hero>& hero);
  uint32_t Unassign(const shared_ptr<const Hero>& hero);
  uint32_t Find(const shared_ptr<const Hero>& hero);
  uint32_t GetNextSelectableNo();
  void     ForEach(ForEachFn);
  Vec2D    GetPosition(const shared_ptr<const Hero>& hero);
  bool     IsReady();
  uint32_t GetNumAssigned();

 private:
  std::pair<Type, DeployElement> FindImpl(const shared_ptr<const Hero>& hero);

 private:
  vector<DeployInfoUnselectable> unselectable_info_list_;
  vector<DeployInfoSelectable> selectable_info_list_;
  uint32_t num_required_;
  AssignmentContainer unselectable_assignment_;
  AssignmentContainer selectable_assignment_;
  std::set<uint32_t> available_no_;
};

} // namespace core
} // namespace mengde

#endif // DEPLOYER_H_

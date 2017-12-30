#ifndef DEPLOYER_H_
#define DEPLOYER_H_

#include "util/common.h"
#include <unordered_set>
#include <set>

class Hero;

struct DeployElement {
  shared_ptr<Hero> hero;
  uint32_t no;

  DeployElement(const shared_ptr<Hero> hero, uint32_t no) : hero(hero), no(no) {}
};

struct DeployerComparer {
  bool operator()(const DeployElement& lhs, const DeployElement& rhs);
};

namespace std
{
  template<>
  struct hash<DeployElement>
  {
    std::size_t operator()(DeployElement const& o) const noexcept
    {
      return std::hash<shared_ptr<Hero>>{}(o.hero);
    }
  };
}

class Deployer {
 public:
  typedef std::unordered_set<DeployElement, std::hash<DeployElement>, DeployerComparer> AssignmentContainer;
  typedef function<void(const DeployElement&)> ForEachFn;

 public:
  Deployer(const vector<Vec2D>&);
  uint32_t Assign(const shared_ptr<Hero>& hero);
  void     Unassign(const shared_ptr<Hero>& hero);
  uint32_t PopNextNo();
  void     ForEach(ForEachFn);
  Vec2D    ConvertToPosition(uint32_t);

 private:
  vector<Vec2D> positions_;
  AssignmentContainer assignment_;
  std::set<uint32_t> available_no_;
};

#endif // DEPLOYER_H_

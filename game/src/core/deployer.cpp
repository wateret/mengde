#include "deployer.h"

namespace mengde {
namespace core {

//
// DeployerComparer
//

bool DeployerComparer::operator()(const DeployElement& lhs, const DeployElement& rhs) {
  // NOTE comparing hero pointer is just enough for this case
  return lhs.hero.get() == rhs.hero.get();
}

//
// Deployer
//

Deployer::Deployer(const vector<DeployInfoUnselectable>& unselectable_info_list,
                   const vector<DeployInfoSelectable>& selectable_info_list, uint32_t num_required)
    : unselectable_info_list_(unselectable_info_list),
      selectable_info_list_(selectable_info_list), num_required_(num_required) {
  for (uint32_t idx = 0; idx < unselectable_info_list_.size(); idx++) {
    const DeployInfoUnselectable& deploy_info = unselectable_info_list_[idx];
    const uint32_t deploy_no = idx + 1;
    uint32_t assigned_no = unselectable_assignment_.size() + 1;
    unselectable_assignment_.insert({deploy_info.hero, deploy_no});
    ASSERT_EQ(assigned_no, deploy_no);
  }
}

// returns 0 if fail to assign
// returns the deploy number otherwise

uint32_t Deployer::Assign(const shared_ptr<Hero>& hero) {
  // TODO check if already exists
  uint32_t no = GetNextSelectableNo();
  if (no != 0) selectable_assignment_.insert({hero, no}); // Assigned successfuly
  return no;
}

uint32_t Deployer::Unassign(const shared_ptr<Hero>& hero) {
  auto found = FindImpl(hero);
  const Type type = found.first;
  const DeployElement& deploy_element = found.second;
  if (type == Type::kUnselectable) {
    // Do not unassign unselectables
    return deploy_element.no;
  } else {
    ASSERT(type == Type::kSelectable);
    available_no_.insert(deploy_element.no);
    selectable_assignment_.erase(deploy_element);
    return 0;
  }
}

uint32_t Deployer::Find(const shared_ptr<Hero>& hero) {
  auto found = FindImpl(hero);
  const Type type = found.first;
  const DeployElement& deploy_element = found.second;
  if (type == Type::kNone) {
    return 0;
  } else {
    return deploy_element.no;
  }
}

uint32_t Deployer::GetNextSelectableNo() {
  if (!available_no_.empty()) {
    uint32_t next = *available_no_.begin();
    available_no_.erase(available_no_.begin());
    return next;
  }
  uint32_t next = selectable_assignment_.size() + 1;
  if (next >= selectable_info_list_.size() + 1) next = 0; // Cannot assign more
  return next;
}

void Deployer::ForEach(ForEachFn fn) {
  std::for_each(unselectable_assignment_.begin(), unselectable_assignment_.end(), fn);
  std::for_each(selectable_assignment_.begin(), selectable_assignment_.end(), fn);
}

Vec2D Deployer::GetPosition(const shared_ptr<Hero>& hero) {
  auto found = FindImpl(hero);
  const Type type = found.first;
  const DeployElement& deploy_element = found.second;
  if (type == Type::kUnselectable) {
    return unselectable_info_list_[deploy_element.no - 1].position;
  } else {
    ASSERT(type == Type::kSelectable);
    return selectable_info_list_[deploy_element.no - 1].position;
  }
}

std::pair<Deployer::Type, DeployElement> Deployer::FindImpl(const shared_ptr<Hero>& hero) {
  DeployElement element(hero, 0 /* Just a dummy number, it is not used for entry comparison */);

  // Find from unselectables first
  {
    auto found = unselectable_assignment_.find(element);
    if (found != unselectable_assignment_.end()) {
      return std::make_pair(Type::kUnselectable, *found);
    }
  }

  // Then selectables
  {
    auto found = selectable_assignment_.find(element);
    if (found != selectable_assignment_.end()) {
      return std::make_pair(Type::kSelectable, *found);
    }
  }

  shared_ptr<Hero> dummy_hero = nullptr;
  return std::make_pair(Type::kNone, DeployElement(dummy_hero, 0));
}

bool Deployer::IsReady() {
  return num_required_ <= selectable_assignment_.size();
}

uint32_t Deployer::GetNumAssigned() {
  return unselectable_assignment_.size() + selectable_assignment_.size();
}

} // namespace mengde
} // namespace core

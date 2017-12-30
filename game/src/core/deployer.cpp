#include "deployer.h"

// DeployerComparer

bool DeployerComparer::operator()(const DeployElement& lhs, const DeployElement& rhs) {
  // NOTE comparing hero pointer is just enough for this case
  return lhs.hero.get() == rhs.hero.get();
}

//
// Deployer
//

Deployer::Deployer(const vector<Vec2D>& positions) : positions_(positions) {
  // Insert dummy entry for index 0 since deploy number starts from 1
  positions_.insert(positions_.begin(), {-1, -1});
}

// returns 0 if fail to assign
// returns the deploy number otherwise

uint32_t Deployer::Assign(const shared_ptr<Hero>& hero) {
  uint32_t no = PopNextNo();
  if (no != 0) assignment_.insert({hero, no}); // Assigned successfuly
  return no;
}

void Deployer::Unassign(const shared_ptr<Hero>& hero) {
  auto found = assignment_.find({hero, 0 /* Just a dummy number, it is not used for entry comparison */});
  ASSERT(found != assignment_.end());
  available_no_.insert(found->no);
  assignment_.erase(found);
}

uint32_t Deployer::PopNextNo() {
  if (!available_no_.empty()) {
    uint32_t next = *available_no_.begin();
    available_no_.erase(available_no_.begin());
    return next;
  }
  uint32_t next = assignment_.size() + 1;
  if (next >= positions_.size()) next = 0; // Cannot assign more
  return next;
}

void Deployer::ForEach(ForEachFn fn) {
  std::for_each(assignment_.begin(), assignment_.end(), fn);
}

Vec2D Deployer::ConvertToPosition(uint32_t index) {
  ASSERT(index < positions_.size());
  return positions_[index];
}

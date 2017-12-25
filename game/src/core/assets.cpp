#include "assets.h"
#include "hero.h"
#include "item.h"
#include <limits.h>

void Money::Pay(const Money& cost) {
  ASSERT(Affordable(cost));
  amount_ -= cost.amount_;
}

void Money::Gain(const Money& money) {
  ASSERT(amount_ > UINT32_MAX - money.amount_); // Check overflow
  amount_ += money.amount_;
}

Assets::Assets() {
}

void Assets::AddHero(Hero* hero) {
  string id = hero->GetId();
  auto found = heroes_.find(id);
  if (found == heroes_.end()) {
    heroes_[id] = hero;
  } else {
    UNREACHABLE("Hero already exists.");
  }
}

void Assets::RemoveHero(const string& id) {
  auto found = heroes_.find(id);
  if (found == heroes_.end()) {
    UNREACHABLE("Hero does not exist.");
  } else {
    heroes_.erase(found);
  }
}

Hero* Assets::GetHero(const string& id) {
  auto found = heroes_.find(id);
  if (found == heroes_.end()) {
    UNREACHABLE("Hero does not exist.");
  } else {
    return found->second;
  }
}

vector<Hero*> Assets::GetHeroes() {
  vector<Hero*> ret;
  for (auto kv : heroes_) {
    ret.push_back(kv.second);
  }
  return ret;
}

void Assets::AddItem(Item* item, uint32_t amount) {
  ASSERT(amount > 0);
  string id = item->GetId();
  auto found = items_.find(id);
  if (found == items_.end()) {
    items_.insert(std::make_pair(id, Amount<Item*>(item, 0)));
    found = items_.find(id);
  }
  found->second.amount += amount;
}

void Assets::RemoveItem(const string& id, uint32_t amount) {
  auto found = items_.find(id);
  if (found == items_.end()) {
    UNREACHABLE("Item does not exist.");
  } else {
    auto& r = found->second;
    ASSERT(r.amount >= amount);
    r.amount -= amount;
    if (r.HasNone()) {
      items_.erase(found);
    }
  }
}

Item* Assets::GetItem(const string& id) {
  auto found = items_.find(id);
  if (found == items_.end()) {
    UNREACHABLE("Item does not exist.");
  } else {
    auto& r = found->second;
    ASSERT(!r.HasNone());
    return r.object; // Return first one as all items in the vector are identical
  }
}

uint32_t Assets::GetAmountItem(const string& id) {
  auto found = items_.find(id);
  if (found == items_.end()) {
    UNREACHABLE("Item does not exist.");
  } else {
    auto& r = found->second;
    ASSERT(!r.HasNone());
    return r.amount;
  }
}

vector<Item*> Assets::GetItems() {
  vector<Item*> ret;
  for (auto kv : items_) {
    ASSERT(!kv.second.HasNone());
    ret.push_back(kv.second.object);
  }
  return ret;
}


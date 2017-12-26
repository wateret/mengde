#include "assets.h"
#include "hero.h"
#include "equipment.h"
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

void Assets::AddHero(shared_ptr<Hero> hero) {
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

shared_ptr<Hero> Assets::GetHero(const string& id) {
  auto found = heroes_.find(id);
  if (found == heroes_.end()) {
    UNREACHABLE("Hero does not exist.");
  } else {
    return found->second;
  }
}

vector<shared_ptr<Hero>> Assets::GetHeroes() {
  vector<shared_ptr<Hero>> ret;
  for (auto kv : heroes_) {
    ret.push_back(kv.second);
  }
  return ret;
}

void Assets::AddEquipment(Equipment* equipment, uint32_t amount) {
  ASSERT(amount > 0);
  string id = equipment->GetId();
  auto found = equipments_.find(id);
  if (found == equipments_.end()) {
    equipments_.insert(std::make_pair(id, Amount<Equipment*>(equipment, 0)));
    found = equipments_.find(id);
  }
  found->second.amount += amount;
}

void Assets::RemoveEquipment(const string& id, uint32_t amount) {
  auto found = equipments_.find(id);
  if (found == equipments_.end()) {
    UNREACHABLE("Equipment does not exist.");
  } else {
    auto& r = found->second;
    ASSERT(r.amount >= amount);
    r.amount -= amount;
    if (r.HasNone()) {
      equipments_.erase(found);
    }
  }
}

Equipment* Assets::GetEquipment(const string& id) {
  auto found = equipments_.find(id);
  if (found == equipments_.end()) {
    UNREACHABLE("Equipment does not exist.");
  } else {
    auto& r = found->second;
    ASSERT(!r.HasNone());
    return r.object; // Return first one as all equipments in the vector are identical
  }
}

uint32_t Assets::GetAmountEquipment(const string& id) {
  auto found = equipments_.find(id);
  if (found == equipments_.end()) {
    UNREACHABLE("Equipment does not exist.");
  } else {
    auto& r = found->second;
    ASSERT(!r.HasNone());
    return r.amount;
  }
}

vector<Equipment*> Assets::GetEquipments() {
  vector<Equipment*> ret;
  for (auto kv : equipments_) {
    ASSERT(!kv.second.HasNone());
    ret.push_back(kv.second.object);
  }
  return ret;
}


#include "assets.h"

#include <limits>

#include "core/equipment_set.h"
#include "equipment.h"
#include "hero.h"

namespace mengde {
namespace core {

void Money::Pay(const Money& cost) {
  ASSERT(Affordable(cost));
  amount_ -= cost.amount_;
}

void Money::Gain(const Money& money) {
  const uint32_t money_limit = std::numeric_limits<uint32_t>::max();
  const uint32_t gain_limit = money_limit - amount_;

  ASSERT_LE(money.amount_, gain_limit);  // Check overflow

  if (money.amount_ <= gain_limit) {
    amount_ += money.amount_;
  } else {
    // NOTE In debug mode this case will fail with assertion check but this code is added for relase mode
    amount_ = money_limit;
  }
}

void Assets::AddHero(unique_ptr<Hero>&& hero) {
  string id = hero->id();
  auto found = heroes_.find(id);
  if (found == heroes_.end()) {
    heroes_[id] = std::move(hero);
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
  auto&& found = heroes_.find(id);
  if (found == heroes_.end()) {
    UNREACHABLE("Hero does not exist.");
    return nullptr;
  } else {
    return found->second.get();
  }
}

vector<const Hero*> Assets::GetHeroes() {
  vector<const Hero*> ret;
  for (auto&& kv : heroes_) {
    ret.push_back(kv.second.get());
  }
  return ret;
}

void Assets::AddEquipment(const Equipment* equipment, uint32_t amount) {
  ASSERT(amount > 0);
  string id = equipment->GetId();
  auto found = equipments_.find(id);
  if (found == equipments_.end()) {
    equipments_.insert(std::make_pair(id, EquipmentWithAmount(equipment, 0)));
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

const Equipment* Assets::GetEquipment(const string& id) {
  auto found = equipments_.find(id);
  if (found == equipments_.end()) {
    UNREACHABLE("Equipment does not exist.");
    return nullptr;
  } else {
    auto& r = found->second;
    ASSERT(!r.HasNone());
    return r.object;
  }
}

uint32_t Assets::GetAmountEquipment(const string& id) {
  auto found = equipments_.find(id);
  if (found == equipments_.end()) {
    UNREACHABLE("Equipment does not exist.");
    return 0;
  } else {
    auto& r = found->second;
    ASSERT(!r.HasNone());
    return r.amount;
  }
}

vector<EquipmentWithAmount> Assets::GetEquipmentsWithAmount() {
  vector<EquipmentWithAmount> ret;
  for (auto kv : equipments_) {
    ASSERT(!kv.second.HasNone());
    ret.push_back(kv.second);
  }
  return ret;
}

vector<const Equipment*> Assets::GetEquipments() {
  vector<const Equipment*> ret;
  for (auto kv : equipments_) {
    ASSERT(!kv.second.HasNone());
    ret.push_back(kv.second.object);
  }
  return ret;
}

void Assets::HeroPutEquipmentOn(Hero* hero, const Equipment* equipment) {
  const Equipment* equipment_new = GetEquipment(equipment->GetId());
  ASSERT(equipment_new != nullptr);
  if (equipment == nullptr || equipment_new == nullptr) return;

  const Equipment* equipment_current = hero->GetEquipmentSet()->GetEquipment(equipment->GetType());
  if (equipment_current != nullptr) {
    AddEquipment(equipment_current, 1);
  }
  RemoveEquipment(equipment_new->GetId(), 1);

  hero->PutOn(equipment_new);
}

Assets::Assets(const Assets& o) : heroes_{}, equipments_{o.equipments_}, money_{o.money_} {
  // Explicitly deep-copy hero elements
  for (auto&& e : o.heroes_) {
    heroes_[e.first] = std::make_unique<Hero>(*e.second.get());
  }
}

}  // namespace core
}  // namespace mengde

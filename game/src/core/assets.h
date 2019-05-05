#ifndef MENGDE_CORE_ASSETS_H_
#define MENGDE_CORE_ASSETS_H_

#include <map>

#include "util/common.h"

namespace mengde {
namespace core {

class Hero;
class Equipment;

class Money {
 public:
  Money() : amount_{0u} {}
  Money(uint32_t amount) : amount_{amount} {}
  Money(const Money&) = default;
  bool Affordable(const Money& cost) const { return (amount_ >= cost.amount_); }
  void Pay(const Money& cost);
  void Gain(const Money& money);
  uint32_t amount() const { return amount_; }

 private:
  uint32_t amount_;
};

template <typename T>
struct Amount {
  T object;
  uint32_t amount;

  Amount(T o, uint32_t a) : object(o), amount(a) {}
  Amount(const Amount<T>&) = default;
  bool HasNone() { return amount == 0; }
};

using EquipmentWithAmount = Amount<const Equipment*>;

class Assets {
 public:
  Assets() = default;
  ~Assets() = default;

 public:
  Assets(const Assets&);
  Assets& operator=(const Assets&) = delete;

 public:
  // Hero related //
  void AddHero(unique_ptr<Hero>&& hero);
  void RemoveHero(const string& id);
  Hero* LookupHero(const string& id);
  const std::map<string, unique_ptr<Hero>>& heroes() const { return heroes_; }
  const std::map<string, EquipmentWithAmount>& equipments() const { return equipments_; }
  vector<const Hero*> GetHeroes();

  // Equipment related //
  void AddEquipment(const Equipment* equipment, uint32_t amount);
  void RemoveEquipment(const string& id, uint32_t amount);
  const Equipment* LookupEquipment(const string& id);
  uint32_t LookupEquipmentAmount(const string& id);
  vector<EquipmentWithAmount> GetEquipmentsWithAmount();
  vector<const Equipment*> GetEquipments();

  // Money related //
  void PayMoney(const Money& cost);
  void GainMoney(const Money& money);
  uint32_t GetMoneyAmount() const { return money_.amount(); }

  // Others //
  void HeroPutEquipmentOn(Hero*, const Equipment*);

 private:
  std::map<string, unique_ptr<Hero>> heroes_;
  std::map<string, EquipmentWithAmount> equipments_;
  //  std::map<string, Amount<Consumable*>>
  Money money_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_ASSETS_H_

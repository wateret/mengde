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
  Money() : amount_(0u) {}
  Money(const Money&) = default;
  bool Affordable(const Money& cost) const { return (amount_ >= cost.amount_); }
  void Pay(const Money& cost);
  void Gain(const Money& money);
  uint32_t GetAmount() { return amount_; }

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

typedef Amount<const Equipment*> EquipmentWithAmount;

class Assets {
 public:
  Assets();
  // NOTE Default copy constructor does exactly what we want for now, but should be careful when members are added
  Assets(const Assets&) = default;
  ~Assets();

  void AddHero(Hero*);
  void RemoveHero(const string&);
  Hero* GetHero(const string&);
  vector<const Hero*> GetHeroes();

  void AddEquipment(const Equipment*, uint32_t);
  void RemoveEquipment(const string&, uint32_t);
  const Equipment* GetEquipment(const string&);
  uint32_t GetAmountEquipment(const string&);
  vector<EquipmentWithAmount> GetEquipmentsWithAmount();
  vector<const Equipment*> GetEquipments();

  void PayMoney(const Money&);
  void GainMoney(const Money&);
  uint32_t GetAmountMoney();

  void HeroPutEquipmentOn(Hero*, const Equipment*);

 private:
  std::map<string, Hero*> heroes_;
  std::map<string, EquipmentWithAmount> equipments_;
  //  std::map<string, Amount<Consumable*>>
  Money money_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_ASSETS_H_

#ifndef ASSETS_H_
#define ASSETS_H_

#include <map>
#include "common.h"

class Hero;
class Item;

class Money {
 public:
   Money() : amount_(0u) {}
   bool Affordable(const Money& cost) const { return (amount_ >= cost.amount_); }
   void Pay(const Money& cost);
   void Gain(const Money& money);
   uint32_t GetAmount() { return amount_; }

 private:
  uint32_t amount_;
};

template <typename T>
struct Amount {
  T        object;
  uint32_t amount;

  Amount(T o, uint32_t a) : object(o), amount(a) {}
  bool HasNone() { return amount == 0; }
};

class Assets {
 public:
  Assets();

  void AddHero(shared_ptr<Hero>);
  void RemoveHero(const string&);
  shared_ptr<Hero> GetHero(const string&);
  vector<shared_ptr<Hero>> GetHeroes();

  void AddItem(Item*, uint32_t);
  void RemoveItem(const string&, uint32_t);
  Item* GetItem(const string&);
  uint32_t GetAmountItem(const string&);
  vector<Item*> GetItems();

  void PayMoney(const Money&);
  void GainMoney(const Money&);
  uint32_t GetAmountMoney();

 private:
  std::map<string, shared_ptr<Hero>> heroes_;
  std::map<string, Amount<Item*>> items_;
//  std::map<string, Amount<Consumable*>>
  Money money_;
};

#endif // ASSETS_H_

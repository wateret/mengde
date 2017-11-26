#ifndef ITEM_H_
#define ITEM_H_

#include <string>
#include "unit_effect_list.h"
#include "common.h"

class EventEffect;
class StatModifier;

class Item {
 public:
  enum ItemType {
    kItemNone,
    kItemWeapon,
    kItemArmor,
    kItemAid
  };

 public:
  Item(const std::string&, ItemType);
  string GetId() { return id_; }
  ItemType GetType() { return type_; }
  void AddModifier(StatModifier*);
  void AddEffect(EventEffect*);
  Stat CalcModifierAddends();
  Stat CalcModifierMultipliers();
  void RaiseEvent(EventEffect::EventEffectType, Unit*, void*);

 private:
  string id_;
  ItemType type_;
  UnitEffectList unit_effect_list_;
};

#endif // ITEM_H_

#ifndef ITEM_H_
#define ITEM_H_

#include <string>
#include "unit_effect_list.h"
#include "common.h"
#include "i_event.h"

class EventEffect;
class StatModifier;

class Item : public IEvent {
 public:
  enum ItemType {
    kItemNone,
    kItemWeapon,
    kItemArmor,
    kItemAid
  };

 public:
  void RaiseEvent(EventEffect::Type, Unit*, void*) override;

 public:
  Item(const std::string&, ItemType);
  string GetId() { return id_; }
  ItemType GetType() { return type_; }
  void AddModifier(StatModifier*);
  void AddEffect(EventEffect*);
  Stat CalcModifierAddends();
  Stat CalcModifierMultipliers();

 private:
  string id_;
  ItemType type_;
  UnitEffectList unit_effect_list_;
};

#endif // ITEM_H_

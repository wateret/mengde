#ifndef ITEM_SLOT_H_
#define ITEM_SLOT_H_

#include "item.h"
#include "stat.h"

class ItemSlot {
 public:
  ItemSlot(Item::ItemType);
  Item* PutItemOn(Item*);
  Stat CalcModifierAddends();
  Stat CalcModifierMultipliers();
  bool IsItemOn() { return item_ != NULL; }
  Item* GetItem() { return item_; }

 private:
  Item::ItemType type_;
  Item* item_;
};

#endif // ITEM_SLOT_H_

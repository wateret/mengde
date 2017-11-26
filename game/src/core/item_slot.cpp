#include "item_slot.h"
#include "common.h"

ItemSlot::ItemSlot(Item::ItemType type)
    : type_(type), item_(NULL) {
}

Item* ItemSlot::PutItemOn(Item* item) {
  if (item->GetType() == type_) {
    Item* cur_item_ = item_;
    item_ = item;
    return cur_item_;
  } else {
    LOG_WARNING("Invalid type of item for the slot.");
    return NULL;
  }
}

Stat ItemSlot::CalcModifierAddends() {
  if (IsItemOn()) {
    return item_->CalcModifierAddends();
  } else {
    return {0, 0, 0, 0, 0};
  }
}

Stat ItemSlot::CalcModifierMultipliers() {
  if (IsItemOn()) {
    return item_->CalcModifierMultipliers();
  } else {
    return {0, 0, 0, 0, 0};
  }
}

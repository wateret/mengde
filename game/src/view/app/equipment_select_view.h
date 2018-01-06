#ifndef EQUIPMENT_SELECT_VIEW_H_
#define EQUIPMENT_SELECT_VIEW_H_

#include "view/uifw/callback_view.h"
#include "view/uifw/composite_view.h"
#include "core/assets.h" // XXX temporary include for EquipmentWithAmount

namespace mengde {
namespace core {
  class Assets;
  class Hero;
}
}

class RowMajorListView;
class IEquipmentSetSetter;

class ItemIconView : public CallbackView {
 public:
  ItemIconView(const Rect&, const string& equipment_id, uint32_t amount);
};

class EquipmentSelectView : public CompositeView {
 public:
  EquipmentSelectView(const Rect&, IEquipmentSetSetter*);
  void SetHero(const shared_ptr<const mengde::core::Hero>& hero) { hero_ = hero; }
  void SetEquipments(const vector<mengde::core::EquipmentWithAmount>&, mengde::core::Assets*);

 private:
  shared_ptr<const mengde::core::Hero> hero_;
  RowMajorListView* equipment_list_view_;
  IEquipmentSetSetter* equipment_set_update_;
};

#endif // EQUIPMENT_SELECT_VIEW_H_

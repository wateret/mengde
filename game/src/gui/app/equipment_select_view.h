#ifndef EQUIPMENT_SELECT_VIEW_H_
#define EQUIPMENT_SELECT_VIEW_H_

#include "core/assets.h"  // XXX temporary include for EquipmentWithAmount
#include "gui/uifw/callback_view.h"
#include "gui/uifw/composite_view.h"

#include "common.h"

namespace mengde {
namespace core {
class Assets;
class Hero;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class IEquipmentSetSetter;

class ItemIconView : public CallbackView {
 public:
  ItemIconView(const Rect&, const string& equipment_id, uint32_t amount);
};

class EquipmentSelectView : public CompositeView {
 public:
  EquipmentSelectView(const Rect&, IEquipmentSetSetter*);
  void SetHero(const core::Hero* hero) { hero_ = hero; }
  void SetEquipments(const vector<core::EquipmentWithAmount>&, core::Assets*);

 private:
  const core::Hero*    hero_;
  RowMajorListView*    equipment_list_view_;
  IEquipmentSetSetter* equipment_set_update_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // EQUIPMENT_SELECT_VIEW_H_

#ifndef MENGDE_GUI_APP_EQUIPMENT_SET_VIEW_H_
#define MENGDE_GUI_APP_EQUIPMENT_SET_VIEW_H_

#include "gui/uifw/callback_view.h"
#include "gui/uifw/composite_view.h"
#include "gui/uifw/layout_helper.h"

#include "common.h"

namespace mengde {
namespace core {
class Unit;
class Equipment;
class EquipmentSet;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {

namespace app {

class EquipmentView : public CallbackView {
 public:
  EquipmentView(const Rect&, const core::Equipment*);
  void SetEquipment(const core::Equipment*);

 private:
  ImageView* iv_image_;
  TextView* tv_name_;
  TextView* tv_desc_;
};

class EquipmentSetView : public CompositeView {
 public:
  EquipmentSetView(const Rect&);
  void SetEquipmentSet(const core::EquipmentSet*);
  void SetWeaponMouseButtonHandler(const CallbackView::MouseButtonHandler&);
  void SetArmorMouseButtonHandler(const CallbackView::MouseButtonHandler&);
  void SetAidMouseButtonHandler(const CallbackView::MouseButtonHandler&);

 private:
  void OnUpdate();

 private:
  const core::EquipmentSet* equipment_set_;
  TextView* tv_weapon_label_;
  TextView* tv_armor_label_;
  TextView* tv_aid_label_;
  EquipmentView* eqv_weapon_;
  EquipmentView* eqv_armor_;
  EquipmentView* eqv_aid_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_EQUIPMENT_SET_VIEW_H_

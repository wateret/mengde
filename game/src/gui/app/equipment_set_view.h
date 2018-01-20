#ifndef EQUIPMENT_SET_VIEW_H_
#define EQUIPMENT_SET_VIEW_H_

#include "gui/uifw/callback_view.h"
#include "gui/uifw/composite_view.h"
#include "gui/uifw/layout_helper.h"
#include "i_equipment_set_setter.h"

namespace mengde {
namespace core {
  class Unit;
  class Equipment;
  class EquipmentSet;
}
}

class ImageView;
class TextView;;

class EquipmentView : public CallbackView {
 public:
  EquipmentView(const Rect*, const mengde::core::Equipment*);
  void SetEquipment(const mengde::core::Equipment*);

 private:
  ImageView* iv_image_;
  TextView*  tv_name_;
  TextView*  tv_desc_;
};

class EquipmentSetView : public CompositeView, public IEquipmentSetSetter {
 public:
  EquipmentSetView(const Rect*);
  void SetEquipmentSet(const mengde::core::EquipmentSet*) override;
  void SetWeaponMouseButtonHandler(const CallbackView::MouseButtonHandler&);
  void SetArmorMouseButtonHandler(const CallbackView::MouseButtonHandler&);
  void SetAidMouseButtonHandler(const CallbackView::MouseButtonHandler&);

 private:
  void OnUpdate();

 private:
  const mengde::core::EquipmentSet* equipment_set_;
  TextView* tv_weapon_label_;
  TextView* tv_armor_label_;
  TextView* tv_aid_label_;
  EquipmentView* eqv_weapon_;
  EquipmentView* eqv_armor_;
  EquipmentView* eqv_aid_;
};

#endif // EQUIPMENT_SET_VIEW_H_

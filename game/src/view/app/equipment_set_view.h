#ifndef EQUIPMENT_SET_VIEW_H_
#define EQUIPMENT_SET_VIEW_H_

#include "view/uifw/callback_view.h"
#include "view/uifw/composite_view.h"
#include "view/foundation/layout_helper.h"
#include "i_equipment_set_setter.h"

class Unit;
class ImageView;
class TextView;;
class Equipment;
class EquipmentSet;

class EquipmentView : public CallbackView {
 public:
  EquipmentView(const Rect*, const Equipment*);
  void SetEquipment(const Equipment*);

 private:
  ImageView* iv_image_;
  TextView*  tv_name_;
  TextView*  tv_desc_;
};

class EquipmentSetView : public CompositeView, public IEquipmentSetSetter {
 public:
  EquipmentSetView(const Rect*);
  void SetEquipmentSet(EquipmentSet*) override;

 private:
  void OnUpdate();

 private:
  EquipmentSet* equipment_set_;
  TextView* tv_weapon_label_;
  TextView* tv_armor_label_;
  TextView* tv_aid_label_;
  EquipmentView* eqv_weapon_;
  EquipmentView* eqv_armor_;
  EquipmentView* eqv_aid_;
};

#endif // EQUIPMENT_SET_VIEW_H_

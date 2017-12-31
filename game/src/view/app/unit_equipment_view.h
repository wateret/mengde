#ifndef UNIT_EQUIPMENT_VIEW_H_
#define UNIT_EQUIPMENT_VIEW_H_

#include "view/uifw/composite_view.h"
#include "view/foundation/layout_helper.h"

class Unit;
class ImageView;
class TextView;;
class Equipment;

class EquipmentView : public CompositeView {
 public:
  EquipmentView(const Rect*, Equipment*);
  void SetEquipment(Equipment*);

 private:
  ImageView* iv_image_;
  TextView*  tv_name_;
  TextView*  tv_desc_;
};

class UnitEquipmentView : public CompositeView {
 public:
  UnitEquipmentView(const Rect*);
  void SetUnit(Unit* unit);

 private:
  void OnUnitUpdate();

 private:
  Unit*     unit_;
  TextView* tv_weapon_label_;
  TextView* tv_armor_label_;
  TextView* tv_aid_label_;
  EquipmentView* eqv_weapon_;
  EquipmentView* eqv_armor_;
  EquipmentView* eqv_aid_;
};

#endif // UNIT_EQUIPMENT_VIEW_H_

#include "unit_equipment_view.h"
#include "core/equipment.h"
#include "core/unit.h"
#include "view/UIFW/image_view.h"
#include "view/UIFW/text_view.h"

//
// EquipmentView
//

EquipmentView::EquipmentView(const Rect* frame, Equipment* equipment) : CompositeView(frame) {
  Rect iv_frame = LayoutHelper::CalcPosition(GetActualFrame().GetSize(),
                                             {32, 32},
                                             LayoutHelper::kAlignLftMid);
  iv_image_ = new ImageView(&iv_frame, "equipments/50-1.bmp"); // FIXME path hardcoded
  Rect tv_name_frame = {32 + 8, 0, 164, 16};
  tv_name_  = new TextView(&tv_name_frame, "");
  Rect tv_desc_frame = {32 + 8, 16, 164, 52};
  tv_desc_  = new TextView(&tv_desc_frame, "");
  AddChild(iv_image_);
  AddChild(tv_name_);
  AddChild(tv_desc_);

  SetEquipment(equipment);
}

void EquipmentView::SetEquipment(Equipment* equipment) {
  string image_path = "equipment/60-1.bmp";
  string name = "No Equipment";
  string desc = "No Desc";
  if (equipment != NULL) {
    image_path = "equipment/" + equipment->GetId() + ".bmp";
    name = equipment->GetId();
    desc = equipment->GetId() + "_desc";
  }

  iv_image_->SetPath(image_path);
  tv_name_->SetText(name);
  tv_desc_->SetText(desc);
}

//
// UnitEquipmentView
//

UnitEquipmentView::UnitEquipmentView(const Rect* frame)
    : CompositeView(frame), unit_(NULL) {
  const int kMargin = 80 + 8;
  Rect tv_frame = {0, 0, 200, 16};
  tv_weapon_label_ = new TextView(&tv_frame, "Weapon");
  tv_frame.Move({0, kMargin});
  tv_armor_label_  = new TextView(&tv_frame, "Armor");
  tv_frame.Move({0, kMargin});
  tv_aid_label_    = new TextView(&tv_frame, "Aid");
  AddChild(tv_weapon_label_);
  AddChild(tv_armor_label_);
  AddChild(tv_aid_label_);

  Equipment* weapon = NULL;
  Equipment* armor = NULL;
  Equipment* aid = NULL;
  if (unit_ != NULL) {
    weapon = unit_->GetWeapon();
    armor = unit_->GetArmor();
    aid = unit_->GetAid();
  }
  Rect equipment_view_frame = {0, 24, 204, 60};
  eqv_weapon_ = new EquipmentView(&equipment_view_frame, weapon);
  equipment_view_frame.Move({0, equipment_view_frame.GetH() + 24});
  eqv_armor_ = new EquipmentView(&equipment_view_frame, armor);
  equipment_view_frame.Move({0, equipment_view_frame.GetH() + 24});
  eqv_aid_ = new EquipmentView(&equipment_view_frame, aid);
  AddChild(eqv_weapon_);
  AddChild(eqv_armor_);
  AddChild(eqv_aid_);
}

void UnitEquipmentView::SetUnit(Unit* unit) {
  unit_ = unit;
  OnUnitUpdate();
}

void UnitEquipmentView::OnUnitUpdate() {
  ASSERT(unit_ != NULL);
  eqv_weapon_->SetEquipment(unit_->GetWeapon());
  eqv_armor_->SetEquipment(unit_->GetArmor());
  eqv_aid_->SetEquipment(unit_->GetAid());
}

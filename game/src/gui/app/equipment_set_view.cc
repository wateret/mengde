#include "equipment_set_view.h"

#include "core/equipment.h"
#include "core/equipment_set.h"
#include "gui/uifw/image_view.h"
#include "gui/uifw/text_view.h"
#include "resource_path.h"

namespace mengde {
namespace gui {
namespace app {

//
// EquipmentView
//

EquipmentView::EquipmentView(const Rect& frame, const core::Equipment* equipment) : CallbackView(frame) {
  Rect iv_frame = LayoutHelper::CalcPosition(GetActualFrame().GetSize(), {32, 32}, LayoutHelper::kAlignLftMid);
  iv_image_ = new ImageView(iv_frame, rcpath::EquipmentModelPath("60-1").ToString());  // FIXME hardcoded
  Rect tv_name_frame = {32 + 8, 0, 164, 16};
  tv_name_ = new TextView(tv_name_frame, "");
  Rect tv_desc_frame = {32 + 8, 16, 164, 52};
  tv_desc_ = new TextView(tv_desc_frame, "");
  AddChild(iv_image_);
  AddChild(tv_name_);
  AddChild(tv_desc_);

  if (equipment != nullptr) SetEquipment(equipment);
}

void EquipmentView::SetEquipment(const core::Equipment* equipment) {
  Path image_path = rcpath::EquipmentModelPath("60-1");
  string name = "No Equipment";
  string desc = "No Desc";
  if (equipment != nullptr) {
    image_path = rcpath::EquipmentModelPath(equipment->GetId());
    name = equipment->GetId();
    desc = equipment->GetId() + "_desc";
  }

  iv_image_->SetPath(image_path.ToString());
  tv_name_->SetText(name);
  tv_desc_->SetText(desc);
}

//
// EquipmentSetView
//

EquipmentSetView::EquipmentSetView(const Rect& frame) : CompositeView(frame), equipment_set_(nullptr) {
  const int kMargin = 80 + 8;
  Rect tv_frame = {0, 0, 200, 16};
  tv_weapon_label_ = new TextView(tv_frame, "Weapon");
  tv_frame.Move({0, kMargin});
  tv_armor_label_ = new TextView(tv_frame, "Armor");
  tv_frame.Move({0, kMargin});
  tv_aid_label_ = new TextView(tv_frame, "Aid");
  AddChild(tv_weapon_label_);
  AddChild(tv_armor_label_);
  AddChild(tv_aid_label_);

  const core::Equipment* weapon = nullptr;
  const core::Equipment* armor = nullptr;
  const core::Equipment* aid = nullptr;
  if (equipment_set_ != nullptr) {
    weapon = equipment_set_->GetWeapon();
    armor = equipment_set_->GetArmor();
    aid = equipment_set_->GetAid();
  }

  Rect equipment_view_frame = {0, 24, 204, 60};
  eqv_weapon_ = new EquipmentView(equipment_view_frame, weapon);
  equipment_view_frame.Move({0, equipment_view_frame.GetH() + 24});
  eqv_armor_ = new EquipmentView(equipment_view_frame, armor);
  equipment_view_frame.Move({0, equipment_view_frame.GetH() + 24});
  eqv_aid_ = new EquipmentView(equipment_view_frame, aid);

  EquipmentView* equipment_views[] = {eqv_weapon_, eqv_armor_, eqv_aid_};

  for (int i = 0; i < 3; i++) {
    EquipmentView* eqv = equipment_views[i];

    eqv->SetMouseButtonHandler([](const foundation::MouseButtonEvent& e) {
      if (e.IsLeftButtonUp()) {
        LOG_DEBUG("Show Equipment Info");
      }
      return true;
    });

    eqv->SetMouseMotionHandler([eqv](const foundation::MouseMotionEvent& e) {
      if (e.IsMotionOver()) {
        eqv->bg_color(COLOR("darkgray"));
      } else {
        ASSERT(e.IsMotionOut());
        eqv->bg_color(COLOR("transparent"));
      }
      return true;
    });

    AddChild(eqv);
  }
}

void EquipmentSetView::SetEquipmentSet(const core::EquipmentSet* equipment_set) {
  equipment_set_ = equipment_set;
  OnUpdate();
}

void EquipmentSetView::SetWeaponMouseButtonHandler(const CallbackView::MouseButtonHandler& fn) {
  eqv_weapon_->SetMouseButtonHandler(fn);
}

void EquipmentSetView::SetArmorMouseButtonHandler(const CallbackView::MouseButtonHandler& fn) {
  eqv_armor_->SetMouseButtonHandler(fn);
}

void EquipmentSetView::SetAidMouseButtonHandler(const CallbackView::MouseButtonHandler& fn) {
  eqv_aid_->SetMouseButtonHandler(fn);
}

void EquipmentSetView::OnUpdate() {
  ASSERT(equipment_set_ != nullptr);
  eqv_weapon_->SetEquipment(equipment_set_->GetWeapon());
  eqv_armor_->SetEquipment(equipment_set_->GetArmor());
  eqv_aid_->SetEquipment(equipment_set_->GetAid());
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

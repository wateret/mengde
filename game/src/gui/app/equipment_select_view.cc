#include "equipment_select_view.h"

#include "core/assets.h"
#include "core/equipment.h"
#include "core/equipment_set.h"
#include "core/hero.h"
#include "equipment_set_view.h"
#include "gui/uifw/image_view.h"
#include "gui/uifw/layout_helper.h"
#include "gui/uifw/row_major_list_view.h"
#include "gui/uifw/text_view.h"

namespace mengde {
namespace gui {
namespace app {

//
// EquipmentIconView
//

ItemIconView::ItemIconView(const Rect& frame, const string& equipment_id, uint32_t amount) : CallbackView(frame) {
  bg_color(COLOR("transparent"));
  padding(8);

  Rect       iv_icon_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {32, 32}, LayoutHelper::kAlignCenter);
  ImageView* iv_icon       = new ImageView(iv_icon_frame, "equipment/" + equipment_id + ".bmp");
  AddChild(iv_icon);

  Rect      tv_amount_frame = GetActualFrame();
  TextView* tv_amount =
      new TextView(&tv_amount_frame, std::to_string(amount), COLOR("white"), 14, LayoutHelper::kAlignRgtTop);
  AddChild(tv_amount);

  SetMouseMotionHandler([this](const foundation::MouseMotionEvent e) {
    if (e.IsMotionOver()) {
      this->bg_color(COLOR("darkgray"));
    } else {
      ASSERT(e.IsMotionOut());
      this->bg_color(COLOR("transparent"));
    }
    return true;
  });
}

//
// EquipmentSelectView
//

EquipmentSelectView::EquipmentSelectView(const Rect& frame, EquipmentSetView* equipment_set_view)
    : CompositeView(frame), hero_(nullptr), equipment_list_view_(nullptr), equipment_set_view_(equipment_set_view) {
  bg_color(COLOR("black"));
}

void EquipmentSelectView::SetEquipments(const vector<core::EquipmentWithAmount>& equipments, core::Assets* assets) {
  static const int kItemSize = 48;

  RowMajorListView* new_list_view = new RowMajorListView(GetActualFrame(), kItemSize);
  ASSERT(hero_ != nullptr);
  if (hero_ == nullptr) return;
  auto hero               = assets->GetHero(hero_->GetId());  // For non-const core::Hero and capture
  auto equipment_set_view = equipment_set_view_;
  for (auto equipment : equipments) {
    ItemIconView* item_icon_view =
        new ItemIconView(Rect(0, 0, kItemSize, kItemSize), equipment.object->GetId(), equipment.amount);
    item_icon_view->SetMouseButtonHandler(
        [this, assets, hero, equipment, equipment_set_view](const foundation::MouseButtonEvent e) {
          if (e.IsLeftButtonUp()) {
            ASSERT(hero != nullptr);
            assets->HeroPutEquipmentOn(hero, equipment.object);
            equipment_set_view->SetEquipmentSet(hero->GetEquipmentSet());
            this->visible(false);
          }
          return true;
        });
    new_list_view->AddElement(item_icon_view);
  }

  if (equipment_list_view_ != nullptr) {
    ReplaceChild(equipment_list_view_, new_list_view);
  } else {
    AddChild(new_list_view);
  }
  equipment_list_view_ = new_list_view;
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

#include "equipment_select_view.h"

#include "core/assets.h"
#include "core/equipment.h"
#include "core/equipment_set.h"
#include "core/hero.h"
#include "deploy_view.h"
#include "equipment_set_view.h"
#include "game_view.h"
#include "gui/uifw/image_view.h"
#include "gui/uifw/layout_helper.h"
#include "gui/uifw/row_major_list_view.h"
#include "gui/uifw/text_view.h"
#include "item_detail_view.h"
#include "resource_path.h"

namespace mengde {
namespace gui {
namespace app {

//
// EquipmentIconView
//

ItemIconView::ItemIconView(GameView* gv, const Rect& frame, const core::EquipmentWithAmount& equipment)
    : CallbackView(frame), gv_(gv) {
  const string& equipment_id = equipment.object->GetId();
  uint32_t amount = equipment.amount;
  bg_color(COLOR("transparent"));
  padding(8);

  Rect iv_icon_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {32, 32}, LayoutHelper::kAlignCenter);
  ImageView* iv_icon = new ImageView(iv_icon_frame, rcpath::EquipmentModelPath(equipment_id).ToString());
  AddChild(iv_icon);

  Rect tv_amount_frame = GetActualFrame();
  TextView* tv_amount =
      new TextView(tv_amount_frame, std::to_string(amount), COLOR("white"), 14, LayoutHelper::kAlignRgtTop);
  AddChild(tv_amount);

  SetMouseMotionHandler([this, equipment](const foundation::MouseMotionEvent& e) {
    DeployView* deploy_view = this->gv_->deploy_view();
    ItemDetailView* item_detail_view = this->gv_->item_detail_tooltip_view();
    if (e.IsMotionOver()) {
      this->bg_color(COLOR("darkgray"));
      // Show Item Detail ToolTip
      // DeployView coords + ItemIcon coords + MouseEvent coords + (ItemIcon height/2)
      Vec2D pos =
          deploy_view->GetActualFrameCoords() + this->GetFrameCoords() + e.GetCoords() + (this->GetFrameSize().y / 2);
      item_detail_view->SetCoords(pos);
      item_detail_view->visible(true);
      item_detail_view->SetItem(equipment.object);
    } else {
      ASSERT(e.IsMotionOut());
      this->bg_color(COLOR("transparent"));
      // Hide Item detail tooltip
      item_detail_view->visible(false);
    }
    return true;
  });
}

//
// EquipmentSelectView
//

EquipmentSelectView::EquipmentSelectView(GameView* gv, const Rect& frame, EquipmentSetView* equipment_set_view)
    : CompositeView(frame),
      gv_(gv),
      hero_(nullptr),
      equipment_list_view_(nullptr),
      equipment_set_view_(equipment_set_view) {
  bg_color(COLOR("black"));
}

void EquipmentSelectView::SetEquipments(const vector<core::EquipmentWithAmount>& equipments, core::Assets* assets) {
  static const int kItemSize = 48;

  RowMajorListView* new_list_view = new RowMajorListView(GetActualFrame(), kItemSize);
  ASSERT(hero_ != nullptr);
  if (hero_ == nullptr) return;
  auto hero = assets->LookupHero(hero_->id());  // For non-const core::Hero and capture
  auto equipment_set_view = equipment_set_view_;
  for (auto equipment : equipments) {
    ItemIconView* item_icon_view = new ItemIconView(gv_, Rect(0, 0, kItemSize, kItemSize), equipment);
    item_icon_view->SetMouseButtonHandler(
        [this, assets, hero, equipment, equipment_set_view](const foundation::MouseButtonEvent& e) {
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

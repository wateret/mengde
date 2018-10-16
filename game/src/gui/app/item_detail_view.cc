#include "item_detail_view.h"
#include "core/equipment.h"
#include "gui/uifw/image_view.h"
#include "gui/uifw/text_view.h"

#include "resource_path.h"

namespace mengde {
namespace gui {
namespace app {

// TODO : param 2, equipment should be changed to 'Item'
ItemDetailView::ItemDetailView(const Rect& frame, const core::Equipment* equipment) : CompositeView(frame) {
  const int kAlpha = 160;
  bg_color({64, 64, 64, kAlpha});
  padding(8);

  Rect iv_frame = LayoutHelper::CalcPosition(GetActualFrame().size(), {32, 32}, LayoutHelper::kAlignLftTop);
  iv_image_ = new ImageView(iv_frame, rcpath::EquipmentModelPath("60-1").ToString());
  Rect tv_name_frame = {32 + 8, 0, 164, 16};
  tv_name_ = new TextView(tv_name_frame);
  Rect tv_desc_frame = {32 + 8, 20, 164, 52};
  tv_desc_ = new TextView(tv_desc_frame);
  AddChild(iv_image_);
  AddChild(tv_name_);
  AddChild(tv_desc_);

  if (equipment != nullptr) SetItem(equipment);
}

// TODO : param equipment should be changed to 'Item'
void ItemDetailView::SetItem(const core::Equipment* equipment) {
  Path image_path = rcpath::EquipmentModelPath("60-1");
  string name = "No Item";
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

}  // namespace app
}  // namespace gui
}  // namespace mengde

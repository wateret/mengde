#include "equipment_select_view.h"
#include "core/equipment.h"
#include "view/foundation/layout_helper.h"
#include "view/uifw/image_view.h"
#include "view/uifw/row_major_list_view.h"

//
// EquipmentIconView
//

ItemIconView::ItemIconView(const Rect& frame, const string& item_id) : CallbackView(frame) {
  SetBgColor(COLOR("yellow"));

  Rect iv_icon_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {32, 32}, LayoutHelper::kAlignCenter);
  ImageView* iv_icon = new ImageView(iv_icon_frame, "equipment/" + item_id + ".bmp");
  AddChild(iv_icon);

  SetMouseButtonHandler([] (const MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      LOG_DEBUG("Show Equipment Info");
    }
    return true;
  });

  SetMouseMotionHandler([this] (const MouseMotionEvent e) {
    if (e.IsMotionOver()) {
      this->SetBgColor(COLOR("darkgray"));
    } else {
      ASSERT(e.IsMotionOut());
      this->SetBgColor(COLOR("yellow"));
    }
    return true;
  });
}

//
// EquipmentSelectView
//

EquipmentSelectView::EquipmentSelectView(const Rect& frame)
    : CompositeView(frame), equipment_list_view_(nullptr) {
  SetBgColor(COLOR("black"));
}

void EquipmentSelectView::SetEquipments(const vector<const Equipment*>& equipments) {
  static const int kItemSize = 48;

  RowMajorListView* new_list_view = new RowMajorListView(GetActualFrame(), kItemSize);
  for (auto e : equipments) {
    new_list_view->AddElement(new ItemIconView(Rect(0, 0, kItemSize, kItemSize), e->GetId()));
  }

  if (equipment_list_view_ != nullptr) {
    ReplaceChild(equipment_list_view_, new_list_view);
  } else {
    AddChild(new_list_view);
  }
  equipment_list_view_ = new_list_view;
}

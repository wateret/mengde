#include "unit_list_view.h"
#include "core/unit.h"
#include "vertical_list_view.h"
#include "button_view.h"

UnitListView::UnitListView(const Rect& frame, const vector<Unit*>& unit_list)
    : CompositeView(frame), unit_list_(unit_list) {
  SetPadding(8);
  SetBgColor(COLOR("darkgray"));

  const int element_height = 24;
  Rect list_view_frame = GetActualFrame();
  list_view_frame.SetH(0); // Initial height of list view is zero
  list_view_ = new VerticalListView(list_view_frame);

  for (auto unit : unit_list_) {
    std::string name = unit->GetId();
    Rect button_frame({0, 0}, {list_view_->GetFrameSize().x, element_height});
    ButtonView* button = new ButtonView(&button_frame, name);
    list_view_->AddElement(button);
  }

  AddChild(list_view_);
}


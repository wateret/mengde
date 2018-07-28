#include "row_major_list_view.h"

namespace mengde {
namespace gui {
namespace uifw {

RowMajorListView::RowMajorListView(const Rect& frame, int row_height)
    : CompositeView(frame), row_height_(row_height), last_x_(0) {
  SetSize({GetFrameSize().x, 0});
  last_x_ = GetFrame().GetW();
}

void RowMajorListView::AddElement(View* e) {
  Vec2D frame_size = GetActualFrameSize();
  Vec2D element_size = e->GetFrameSize();

  ASSERT_EQ(row_height_, element_size.y);

  if (last_x_ + element_size.x > frame_size.x) {
    e->SetCoords({0, frame_size.y});
    frame_size += {0, row_height_};
    this->SetSize(frame_size);
    last_x_ = element_size.x;
  } else {
    e->SetCoords({last_x_, frame_size.y - row_height_});
    last_x_ += element_size.x;
  }

  AddChild(e);
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#include "vertical_list_view.h"

namespace mengde {
namespace gui {
namespace uifw {

VerticalListView::VerticalListView(const Rect& frame) : CompositeView(frame), margin_(0), total_element_height_(0) {
  // Initial height must be zero, the height value from frame is ignored
  size({GetFrameSize().x, 0});
}

void VerticalListView::AddElement(View* e) {
  Vec2D frame_size = GetFrameSize();
  Vec2D element_size = e->GetFrameSize();

  // Restriction : element's height must be equal to frame
  // TODO remove the restriction
  ASSERT_EQ(GetActualFrameSize().x, element_size.x);

  e->SetCoords({0, total_element_height_ + margin_});  // Move the coords of frame by the element's height
  total_element_height_ += element_size.y + margin_;
  frame_size = {frame_size.x,
                total_element_height_ + padding() * 2};  // Increment size of the frame by the element's height
  this->size(frame_size);

  AddChild(e);
}

void VerticalListView::Clear() {
  RemoveAllChildren();
  total_element_height_ = 0;
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

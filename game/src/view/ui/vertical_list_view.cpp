#include "vertical_list_view.h"

VerticalListView::VerticalListView(const Rect& frame) : CompositeView(frame) {
  SetPadding(0);

  // Restriction : initial height must be zero
  ASSERT_EQ(GetFrameSize().y, 0);
}

void VerticalListView::AddElement(View* e) {
  Vec2D frame_size   = GetFrameSize();
  Vec2D element_size = e->GetFrameSize();

  // Restriction : element's height must be equal to frame
  // TODO remove the restriction
  ASSERT_EQ(frame_size.x, element_size.x);

  e->SetCoords({0, frame_size.y});   // Move the coords of frame by the element's height
  frame_size += {0, element_size.y}; // Increment size of the frame by the element's height
  this->SetSize(frame_size);

  AddChild(e);
}

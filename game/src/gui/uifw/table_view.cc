#include "table_view.h"
#include "text_view.h"

namespace mengde {
namespace gui {
namespace uifw {

const int TableView::HEADER_HEIGHT = 20;

TableView::TableView(const Rect& frame) : CompositeView(frame), total_element_height_(HEADER_HEIGHT) {
  size({GetFrameSize().x, 0});
}

int TableView::AddCol(const string& title) {
  Vec2D frame_size = GetFrameSize();
  int element_width = frame_size.x / (headers_.size() + 1);
  int total_header_x = 0;
  Rect element_rect = {0, 0, element_width, HEADER_HEIGHT};

  View* element = new TextView{element_rect, title};

  headers_.emplace_back(element);
  for (auto header : headers_) {
    header->SetCoords({total_header_x, 0});
    total_header_x += element_width;
  }

  frame_size = {frame_size.x, HEADER_HEIGHT};
  this->size(frame_size);

  AddChild(element);

  return headers_.size() - 1;
}

int TableView::AddRow(std::initializer_list<View*> views) {
  ASSERT_GE(headers_.size(), views.size());
  rows_.emplace_back(views);

  Vec2D frame_size = GetFrameSize();
  int element_width = frame_size.x / headers_.size();
  int element_height = 0;
  int total_element_x = 0;

  for (auto view : views) {
    element_height = view->GetFrameSize().y;
    view->SetCoords({total_element_x, total_element_height_});
    view->size({element_width, element_height});
    total_element_x += element_width;
  }

  total_element_height_ += element_height;

  frame_size = {frame_size.x, total_element_height_ + padding() * 2};
  this->size(frame_size);

  for (auto view : views) {
    AddChild(view);
  }
  return rows_.size() - 1;
}

int TableView::GetColSize() { return headers_.size(); }
int TableView::GetRowSize() { return rows_.size(); }

void TableView::Clear() {
  RemoveAllChildren();
  headers_.clear();
  rows_.clear();
  total_element_height_ = HEADER_HEIGHT;
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#include "tab_view.h"

#include "button_view.h"
#include "layout_helper.h"
#include "util/common.h"

namespace mengde {
namespace gui {
namespace uifw {

TabView::TabView(const Rect* frame) : CompositeView(frame), view_index_(0) {
  bg_color(COLOR("darkgray"));
  padding(LayoutHelper::kDefaultSpace);
}

void TabView::SetViewIndex(int idx) {
  ASSERT(idx < GetNumTabs());

  // Uncheck
  v_tab_buttons_[view_index_]->check(false);
  v_tabs_[view_index_]->visible(false);

  // Check
  v_tab_buttons_[idx]->check(true);
  v_tabs_[idx]->visible(true);

  view_index_ = idx;
}

void TabView::AddTab(const string& button_text, View* view) {
  // Generate a button for the new tab
  const int   kButtonWidth  = 60;
  const int   kButtonHeight = 20;
  const int   index         = GetNumTabs();
  Rect        btn_frame  = {(kButtonWidth + LayoutHelper::kDefaultSpace / 2) * index, 0, kButtonWidth, kButtonHeight};
  ButtonView* tab_button = new ButtonView(&btn_frame, button_text);
  tab_button->SetMouseButtonHandler([=](const foundation::MouseButtonEvent e) -> bool {
    if (e.IsLeftButtonDown()) {
      this->SetViewIndex(index);
    }
    return true;
  });

  view->visible(false);

  v_tab_buttons_.push_back(tab_button);
  v_tabs_.push_back(view);
  AddChild(tab_button);
  AddChild(view);

  if (index == view_index_) {
    SetViewIndex(view_index_);
  }
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

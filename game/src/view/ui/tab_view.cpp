#include "tab_view.h"
#include "button_view.h"
#include "layout_helper.h"

TabView::TabView(const Rect* frame)
    : CompositeView(frame), view_index_(0) {
  SetBgColor(COLOR_DARKGRAY);
  SetPadding(LayoutHelper::kDefaultSpace);
}

void TabView::SetViewIndex(int idx) {
  view_index_ = idx;
  const int num_tabs = GetNumTabs();
  for (int i = 0; i < num_tabs; i++) {
    v_tabs_[i]->SetVisible(i == idx);
  }
}

void TabView::AddTab(const string& button_text, View* view) {
  const int kButtonWidth = 60;
  const int kButtonHeight = 20;
  const int index = GetNumTabs();
  Rect btn_frame = {(kButtonWidth + LayoutHelper::kDefaultSpace) * index,
                    0,
                    kButtonWidth,
                    kButtonHeight};
  ButtonView* tab_button = new ButtonView(&btn_frame, button_text);
  tab_button->SetMouseButtonHandler([this, index] (const MouseButtonEvent e) -> bool {
    if (e.IsLeftButtonDown()) {
      this->SetViewIndex(index);
      return true;
    }
    return true;
  });

  v_tab_buttons_.push_back(tab_button);
  v_tabs_.push_back(view);
  AddChild(tab_button);
  AddChild(view);

  view->SetVisible(index == view_index_);
}

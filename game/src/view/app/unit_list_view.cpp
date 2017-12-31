#include "unit_list_view.h"
#include "core/unit.h"
#include "view/uifw/vertical_list_view.h"
#include "view/uifw/button_view.h"
#include "view/uifw/scroll_view.h"
#include "view/uifw/text_view.h"

UnitListView::UnitDetailView::UnitDetailView(const Rect& frame)
    : CompositeView(frame), unit_(nullptr), tv_name_(nullptr) {
  Rect tv_name_frame({0, 0}, {200, 100});
  tv_name_ = new TextView(&tv_name_frame);
  AddChild(tv_name_);
}

void UnitListView::UnitDetailView::SetUnit(Unit* unit) {
  unit_ = unit;

  string name = unit->GetId();
  tv_name_->SetText(name);
}

UnitListView::UnitListView(const Rect& frame, const vector<Unit*>& unit_list)
    : CompositeView(frame), unit_list_(unit_list), unit_detail_view_(nullptr) {
  SetPadding(8);
  SetBgColor(COLOR("darkgray"));

  const int margin = 8;
  const Vec2D btn_close_size = {150, 24};
  const Vec2D list_view_size = {btn_close_size.x, GetActualFrameSize().y - (btn_close_size.y + margin)};

  {
    Rect btn_close_frame({0, list_view_size.y + margin}, btn_close_size);
    ButtonView* btn_close = new ButtonView(&btn_close_frame, "Close");
    btn_close->SetMouseButtonHandler([this] (const MouseButtonEvent e) {
      if (e.IsLeftButtonUp()) {
        this->SetVisible(false);
      }
      return true;
    });
    AddChild(btn_close);
  }

  {
    const int element_height = 24;
    Rect list_view_frame({0, 0}, list_view_size);
    VerticalListView* list_view = new VerticalListView(list_view_frame);
    for (auto unit : unit_list_) {
      std::string name = unit->GetId();
      Rect button_frame({0, 0}, {list_view_size.x, element_height});
      ButtonView* button = new ButtonView(&button_frame, name);
      button->SetMouseButtonHandler([this, unit] (const MouseButtonEvent e) {
        if (e.IsLeftButtonUp()) {
          this->SetUnit(unit);
        }
        return true;
      });
      list_view->AddElement(button);
    }

    View* list_view_wrap = new ScrollView(list_view_frame, list_view);
    list_view_wrap->SetBgColor(COLOR("navy"));
    AddChild(list_view_wrap);
  }

  {
    const int udv_frame_x = list_view_size.x + 8;
    Rect cv_frame(udv_frame_x, 0, GetActualFrameSize().x - udv_frame_x, GetActualFrameSize().y);
    unit_detail_view_ = new UnitDetailView(cv_frame);
    unit_detail_view_->SetBgColor(COLOR("gray"));
    AddChild(unit_detail_view_);
  }
}


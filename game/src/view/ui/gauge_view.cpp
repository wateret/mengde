#include "gauge_view.h"
#include "../drawer.h"

GaugeView::GaugeView(const Rect* frame,
                     int cur_val,
                     int max_val,
                     Color cur_color,
                     Color max_color)
    : GaugeView(frame,
                cur_val,
                max_val,
                0,
                cur_color,
                max_color,
                {0, 0, 0, 0}) {
}

GaugeView::GaugeView(const Rect* frame,
                     int cur_val,
                     int max_val,
                     int ext_val,
                     Color cur_color,
                     Color max_color,
                     Color ext_color)
    : View(frame),
      cur_val_(cur_val),
      max_val_(max_val),
      ext_val_(ext_val),
      cur_color_(cur_color),
      max_color_(max_color),
      ext_color_(ext_color),
      help_text_type_(kHelpTextNone),
      help_text_align_(LayoutHelper::kAlignCenter) {
  if (cur_val_ < 0)
    cur_val_ = 0;
}

GaugeView::~GaugeView() {
}

int GaugeView::CalcWidth(int val) {
  ASSERT(max_val_ > 0);
  int w = GetFrame()->GetW();
  return val * w / max_val_;
}

void GaugeView::Render(Drawer* drawer) {
  Rect frame = *GetFrame();
  drawer->SetDrawColor(max_color_);
  drawer->FillRect(&frame);

  int ext_x = CalcWidth(cur_val_ - ext_val_);

  Rect cur_rect = frame;
  cur_rect.SetW(ext_x);
  drawer->SetDrawColor(cur_color_);
  drawer->FillRect(&cur_rect);

  if (ext_val_ > 0) {
    Rect ext_rect = frame;
    ext_rect.SetX(ext_rect.GetX() + ext_x);
    ext_rect.SetW(CalcWidth(cur_val_) - ext_x);
    drawer->SetDrawColor(ext_color_);
    drawer->FillRect(&ext_rect);
  }

  if (help_text_type_ != kHelpTextNone) {
    std::string str_hp;
    switch (help_text_type_) {
      case kHelpTextCurMax:
        str_hp = std::to_string(cur_val_) + " / " + std::to_string(max_val_);
        break;

      case kHelpTextCurOnly:
        str_hp = std::to_string(cur_val_);
        break;

      case kHelpTextNone:
      default:
        UNREACHABLE("Invalid help text type");
        break;
    }
    frame.Contract(4);
    drawer->DrawText(str_hp, 14, {255, 255, 255, 255}, &frame, help_text_align_);
  }
}

#include "unit_dialog_view.h"
#include "text_view.h"
#include "image_view.h"
#include "core/unit.h"

UnitDialogView::UnitDialogView(const Rect* frame, const string& message, Unit* unit)
    : CompositeView(frame),
      message_(message),
      unit_(unit),
      frame_view_(nullptr),
      iv_portrait_(nullptr),
      tv_name_(nullptr),
      tv_message_(nullptr) {
  Rect dialog_frame = LayoutHelper::CalcPosition(GetFrameSize(),
                                                 {360, 120},
                                                 LayoutHelper::kAlignCenter,
                                                 LayoutHelper::kDefaultSpace);
  frame_view_ = new CompositeView(&dialog_frame);
  frame_view_->SetBgColor({64, 64, 64, 192});

  Rect portrait_frame = LayoutHelper::CalcPosition(frame_view_->GetFrameSize(),
                                                   {64, 80},
                                                   LayoutHelper::kAlignLftTop,
                                                   LayoutHelper::kDefaultSpace);
  iv_portrait_ = new ImageView(&portrait_frame);

  Vec2D message_pos = portrait_frame.GetPos();
  Rect message_frame(message_pos + Vec2D(64+8, 0), frame_view_->GetFrameSize());
  tv_message_ = new TextView(&message_frame, message_);
  tv_message_->SetAlign(LayoutHelper::kAlignLftTop);

  Rect name_frame(message_pos + Vec2D(0, 80+4), {64, 20});
  tv_name_ = new TextView(&name_frame, "");
  tv_name_->SetAlign(LayoutHelper::kAlignCenter);
  tv_name_->SetColor(COLOR_YELLOW);

  frame_view_->AddChild(tv_name_);
  frame_view_->AddChild(tv_message_);
  frame_view_->AddChild(iv_portrait_);
  AddChild(frame_view_);
}

void UnitDialogView::SetText(const string& s) {
  tv_message_->SetText(s);
}

void UnitDialogView::SetUnit(Unit* u) {
  unit_ = u;
  string portrait_path = "portrait/" + unit_->GetId() + ".bmp";
  iv_portrait_->SetPath(portrait_path);
  tv_name_->SetText(unit_->GetId());
}

bool UnitDialogView::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    SetVisible(false);
  }
  return true;
}

bool UnitDialogView::OnMouseMotionEvent(const MouseMotionEvent) {
  return true;
}


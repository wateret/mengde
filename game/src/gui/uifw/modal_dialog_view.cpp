#include "modal_dialog_view.h"
#include "text_view.h"
#include "layout_helper.h"

namespace mengde {
namespace gui {
namespace uifw {

ModalDialogView::ModalDialogView(const Rect* frame, const string& message)
    : CompositeView(frame), message_(message), message_view_(nullptr) {
  Rect dialog_frame = LayoutHelper::CalcPosition(GetFrameSize(),
                                                 GetFrameSize() / 3, // FIXME size should be flexible with text size
                                                 LayoutHelper::kAlignCenter,
                                                 LayoutHelper::kDefaultSpace);
  frame_view_ = new CompositeView(&dialog_frame);
  frame_view_->bg_color({64, 64, 64, 192});
  Rect message_frame({0, 0}, frame_view_->GetFrameSize());
  message_view_ = new TextView(&message_frame, message_);
  message_view_->SetAlign(LayoutHelper::kAlignCenter);
  frame_view_->AddChild(message_view_);
  AddChild(frame_view_);
}

void ModalDialogView::SetText(const string& s) {
  message_view_->SetText(s);
}

bool ModalDialogView::OnMouseButtonEvent(const foundation::MouseButtonEvent e) {
  if (e.IsLeftButtonUp()) {
    visible(false);
  }
  return true;
}

bool ModalDialogView::OnMouseMotionEvent(const foundation::MouseMotionEvent) {
  return true;
}

} // namespace uifw
} // namespace gui
} // namespace mengde

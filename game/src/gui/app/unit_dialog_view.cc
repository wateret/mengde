#include "unit_dialog_view.h"

#include "core/unit.h"
#include "gui/uifw/drawer.h"
#include "gui/uifw/image_view.h"
#include "gui/uifw/text_view.h"
#include "resource_path.h"

namespace mengde {
namespace gui {
namespace app {

UnitDialogView::UnitDialogView(const Rect& frame, const string& message, const core::Unit* unit)
    : CompositeView(frame), message_(""), iv_portrait_(nullptr), tv_name_(nullptr), tv_message_(nullptr) {
  bg_color({64, 64, 64, 192});

  Rect portrait_frame =
      LayoutHelper::CalcPosition(GetFrameSize(), {64, 80}, LayoutHelper::kAlignLftTop, LayoutHelper::kDefaultSpace);
  iv_portrait_ = new ImageView(portrait_frame);

  Vec2D message_pos = portrait_frame.pos();
  Rect message_frame(message_pos + Vec2D(64 + 8, 0), GetFrameSize());
  tv_message_ = new TextView(message_frame, message_);
  tv_message_->SetAlign(LayoutHelper::kAlignLftTop);

  Rect name_frame(message_pos + Vec2D(0, 80 + 4), {64, 20});
  tv_name_ = new TextView(name_frame, "");
  tv_name_->SetAlign(LayoutHelper::kAlignCenter);
  tv_name_->SetColor(COLOR("yellow"));

  AddChild(tv_name_);
  AddChild(tv_message_);
  AddChild(iv_portrait_);

  SetText(message);
  SetUnit(unit);
}

void UnitDialogView::SetText(const string& s) { tv_message_->SetText(s); }

void UnitDialogView::SetUnit(const core::Unit* u) {
  if (u == nullptr) return;

  Path portrait_path = rcpath::PortraitPath(u->id());
  iv_portrait_->SetPath(portrait_path.ToString());
  tv_name_->SetText(u->id());
}

bool UnitDialogView::OnMouseButtonEvent(const foundation::MouseButtonEvent&) {
  // Do not handle mouse button event. Just delegate to the View in next priority
  return false;
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

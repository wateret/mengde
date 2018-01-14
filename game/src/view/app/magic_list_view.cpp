#include "magic_list_view.h"
#include "core/magic_list.h"
#include "core/magic.h"
#include "core/unit.h"
#include "root_view.h"
#include "state_ui.h"
#include "view/foundation/drawer.h"
#include "view/uifw/button_view.h"
#include "view/uifw/text_view.h"
#include "view/uifw/scroll_view.h"
#include "view/uifw/vertical_list_view.h"

MagicListView::MagicListView(const Rect& frame,
                             mengde::core::Game* const game,
                             RootView* const rv)
    : CompositeView(frame),
      game_(game),
      rv_(rv),
      item_height_(24),
      lv_magics_(nullptr),
      lv_magics_wrap_(nullptr) {
  SetBgColor(COLOR("darkgray", 160));
  SetPadding(LayoutHelper::kDefaultSpace);

  {
    Rect frame(0, 0, GetActualFrameSize().x, kTitleHeight);
    tv_title_ = new TextView(&frame, "Choose Magic", COLOR("yellow"), 15, LayoutHelper::kAlignCenter);
    this->AddChild(tv_title_);
  }
}

MagicListView::~MagicListView() {
}

void MagicListView::SetUnitAndMagicList(mengde::core::Unit* unit, shared_ptr<mengde::core::MagicList> magic_list) {
  ASSERT((unit == nullptr) == (magic_list == nullptr)); // Both should be null or non-null at the same time

  if (unit == nullptr || magic_list == nullptr) return;

  if (lv_magics_wrap_ != nullptr) {
    this->RemoveChild(lv_magics_wrap_);
  }

  Vec2D frame_size = GetActualFrameSize();

  {
    Rect frame(0, kTitleHeight, frame_size.x, 0);
    lv_magics_ = new VerticalListView(frame);
  }

  {
    const int kPositionFromTitle = kTitleHeight + LayoutHelper::kDefaultSpace;
    Rect frame(0, kPositionFromTitle, frame_size.x, frame_size.y - kPositionFromTitle);
    lv_magics_wrap_ = new ScrollView(frame, lv_magics_);
    this->AddChild(lv_magics_wrap_);
  }

  for (int i = 0, sz = magic_list->NumMagics(); i < sz; i++) {
    mengde::core::Magic* magic = magic_list->GetMagic(i);
    string id    = magic->GetId();
    string name  = magic->GetId();

    // Variables to be captured for callback
    mengde::core::Game*     game = game_;
    RootView* rv   = rv_;

    Rect button_frame({0, 0}, {frame_size.x, item_height_});
    ButtonView* button = new ButtonView(&button_frame, name);
    button->SetMouseButtonHandler([game, rv, unit, id] (const MouseButtonEvent e) {
      if (e.IsLeftButtonUp()) {
        rv->PushUIState(new StateUITargeting({game, rv}, unit, id));
        return true;
      }
      return false;
    });
    lv_magics_->AddElement(button);
  }
}

bool MagicListView::OnMouseButtonEvent(const MouseButtonEvent e) {
  // Refuse to handle RightButtonUp
  if (e.IsRightButtonUp()) {
    return false;
  }
  return CompositeView::OnMouseButtonEvent(e);
}

#include "magic_list_view.h"
#include "core/magic.h"
#include "core/magic_list.h"
#include "core/unit.h"
#include "game_view.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/drawer.h"
#include "gui/uifw/scroll_view.h"
#include "gui/uifw/text_view.h"
#include "gui/uifw/vertical_list_view.h"
#include "state_ui.h"

namespace mengde {
namespace gui {
namespace app {

MagicListView::MagicListView(const Rect& frame, core::Stage* game, core::UserInterface* gi, GameView* gv)
    : CompositeView(frame),
      stage_(game),
      gi_(gi),
      gv_(gv),
      item_height_(24),
      lv_magics_(nullptr),
      lv_magics_wrap_(nullptr) {
  bg_color(COLOR("darkgray", 212));
  padding(LayoutHelper::kDefaultSpace);

  {
    Rect frame(0, 0, GetActualFrameSize().x, kTitleHeight);
    tv_title_ = new TextView(frame, "Choose Magic", COLOR("yellow"), 15, LayoutHelper::kAlignCenter);
    this->AddChild(tv_title_);
  }
}

MagicListView::~MagicListView() {}

void MagicListView::SetData(const core::UnitKey& ukey, const core::MoveKey& mkey,
                            shared_ptr<core::MagicList> magic_list) {
  // TODO For decorators, who should remove the wrapped object? Manually managing it is too complicated
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
    core::Magic* magic = magic_list->GetMagic(i);
    string id = magic->GetId();
    string name = magic->GetId();

    // Variables to be captured for callback
    StateUI::Base base = {stage_, gi_, gv_};

    Rect button_frame({0, 0}, {frame_size.x, item_height_});
    ButtonView* button = new ButtonView(&button_frame, name);
    button->SetMouseButtonHandler([base, ukey, mkey, id](const foundation::MouseButtonEvent& e) {
      if (e.IsLeftButtonUp()) {
        base.gv->PushUIState(new StateUITargeting(base, ukey, mkey, id));
        return true;
      }
      return false;
    });
    lv_magics_->AddElement(button);
  }
}

bool MagicListView::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  // Refuse to handle RightButtonUp
  if (e.IsRightButtonUp()) {
    return false;
  }
  return CompositeView::OnMouseButtonEvent(e);
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

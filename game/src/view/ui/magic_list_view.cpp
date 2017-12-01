#include "magic_list_view.h"
#include "app.h"
#include "drawer.h"
#include "magic_list.h"
#include "magic.h"
#include "unit.h"
#include "root_view.h"
#include "view/state_ui.h"

MagicListView::MagicListView(const Rect* frame,
                             Unit* unit,
                             MagicList* magic_list)
    : CallbackView(frame),
      unit_(unit),
      magic_list_(magic_list),
      item_height_(20),
      hover_index_(-1) {
  ASSERT(unit_ || !magic_list_);
  SetBgColor(COLOR_DARKGRAY);
  SetPadding(LayoutHelper::kDefaultSpace);

  SetMouseMotionHandler([this] (const MouseMotionEvent e) {
    Rect actual_frame = *this->GetFrame();
    actual_frame.SetPos({0, 0});
    Vec2D mouse_coords = e.GetCoords();
    if (actual_frame.Contains(mouse_coords)) {
      Vec2D frame_coords = actual_frame.GetPos();
      Vec2D coords = mouse_coords - frame_coords;
      int idx = coords.y / this->GetItemHeight();
      this->SetHoverIndex(idx);
      return true;
    } else {
      this->SetHoverIndex(-1);
    }
    return false;
  });
}

MagicListView::~MagicListView() {
  Cleanup();
}

int MagicListView::NumItems() {
  return magic_list_->NumMagics();
}

string MagicListView::GetHoverItem() {
  ASSERT(hover_index_ < NumItems());
  return magic_list_->GetMagic(hover_index_)->GetId();
}

void MagicListView::SetUnitAndMagicList(Unit* unit, MagicList* ml) {
  unit_ = unit;
  Cleanup();
  magic_list_ = ml;
}

void MagicListView::SetMouseUpHandler(Game* game, RootView* rv) {
  SetMouseButtonHandler([game, rv, this] (const MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      if (0 <= this->GetHoverIndex() && this->GetHoverIndex()< this->NumItems()) {
        std::string item_str = this->GetHoverItem();
        rv->PushUIState(new StateUIAction({game, rv}, unit_, item_str));
        return true;
      }
    }
    return false;
  });
}

void MagicListView::Render(Drawer* drawer) {
  ASSERT((unit_ && magic_list_) || (!unit_ && !magic_list_));
  if (magic_list_ == nullptr) return;
  // FIXME Remove this RENDER_BEGIN/END workaround
  RENDER_BEGIN(this);
  Rect item_frame(Vec2D(0, 0), GetActualFrameSize());
  item_frame += {0, item_height_ - item_frame.GetH()};
  for (int i = 0, sz = magic_list_->NumMagics(); i < sz; i++) {
    if (i == hover_index_) {
      Rect r = item_frame;
      drawer->SetDrawColor(COLOR_GRAY);
      drawer->FillRectAbs(&r);
    }
    Magic* magic = magic_list_->GetMagic(i);
    drawer->DrawTextAbs(magic->GetId(), 14, COLOR_WHITE, &item_frame, LayoutHelper::kAlignCenter);
    item_frame.Move(0, item_height_);
  }
  RENDER_END(this);
}

void MagicListView::Cleanup() {
  if (magic_list_ != nullptr) {
    delete magic_list_;
  }
}

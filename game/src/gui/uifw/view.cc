#include "view.h"

#include "drawer.h"

namespace mengde {
namespace gui {
namespace uifw {

View::View(const Rect& frame) : frame_(frame), bg_color_(0, 0, 0, 0), padding_(0), visible_(true) {}

View::View() : View(Rect()) {}

Rect View::GetActualFrame() const { return Rect(Vec2D(0, 0), GetActualFrameSize()); }

Vec2D View::GetFrameSize() const { return {frame_.w(), frame_.h()}; }

Vec2D View::GetActualFrameSize() const {
  int a = padding_ * 2;
  return {frame_.w() - a, frame_.h() - a};
}

Vec2D View::GetFrameCoords() const { return {frame_.x(), frame_.y()}; }

Vec2D View::GetActualFrameCoords() const { return {frame_.x() + padding_, frame_.y() + padding_}; }

void View::visible(bool b) {
  if (visible_ == b) return;

  visible_ = b;

  if (!visible_) {
    // To reset if there was hovered objects
    OnMouseMotionEvent({foundation::MouseMotionEvent::Type::kOut});
  }
}

/*
void View::Render(Drawer* drawer) {
  if (!visible_) return;

  const Rect* frame = GetFrame();
  Rect actual_frame = *frame;

  drawer->SetDrawColor(bg_color_);
  drawer->FillRect(frame);
  actual_frame.Contract(padding_);
  drawer->SetViewport(&actual_frame);

  Render(drawer);

  drawer->ResetViewport();
}
*/

bool View::RenderBegin(Drawer* drawer) {
  if (!visible_) return false;

  const Rect& frame = GetFrame();
  Rect actual_frame = frame;
  actual_frame.Contract(padding_);

  drawer->SetDrawColor(bg_color_);
  drawer->FillRect(frame);
  drawer->SetViewport(&actual_frame);

  return true;
}

void View::RenderEnd(Drawer* drawer) { drawer->ResetViewport(); }

bool View::DelegateMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  if (visible_ && GetFrame().Contains(e.GetCoords())) {
    Vec2D conv_coords = e.GetCoords() - GetActualFrameCoords();
    foundation::MouseButtonEvent ec(e.GetButton(), e.GetState(), conv_coords);
    if (OnMouseButtonEvent(ec)) return true;
  }
  return false;
}

bool View::DelegateMouseMotionEvent(const foundation::MouseMotionEvent& e) {
  if (!visible_) return false;
  if (e.IsMotionOver()) {
    if (GetFrame().Contains(e.GetCoords())) {
      Vec2D conv_coords = e.GetCoords() - GetActualFrameCoords();
      foundation::MouseMotionEvent ec(MouseMotionEvent::Type::kOver, conv_coords, e.GetCoordsRel());
      return OnMouseMotionEvent(ec);
    } else {
      // Not over the view but was over in previous event then generate MouseOut event
      if (GetFrame().Contains(e.GetLastCoords())) {
        foundation::MouseMotionEvent ec(MouseMotionEvent::Type::kOut);
        OnMouseMotionEvent(ec);
        return false;  // Always return false to keep delegating
      }
    }
  } else {
    ASSERT(e.IsMotionOut());
    return OnMouseMotionEvent(e);
  }
  return false;
}

bool View::DelegateMouseWheelEvent(const foundation::MouseWheelEvent& e) {
  // FIXME This is same routine with DelegateMouseButtonEvent
  if (visible_ && GetFrame().Contains(e.GetCoords())) {
    Vec2D conv_coords = e.GetCoords() - GetActualFrameCoords();
    foundation::MouseWheelEvent ec(e, conv_coords);
    if (OnMouseWheelEvent(ec)) return true;
  }
  return false;
}

bool View::DelegateKeyEvent(const foundation::KeyEvent& e) {
  if (visible_) {
    if (OnKeyEvent(e)) return true;
  }
  return false;
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

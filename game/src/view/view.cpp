#include "view.h"
#include "drawer.h"

View::View(const Rect* frame)
 : frame_(*frame), bg_color_(0, 0, 0, 0), padding_(0), visible_(true) {
}

View::View(const Rect& frame)
 : frame_(frame), bg_color_(0, 0, 0, 0), padding_(0), visible_(true) {
}

View::View()
 : frame_(), bg_color_(0, 0, 0, 0), padding_(0), visible_(true) {
}

Rect View::GetActualFrame() const {
  return Rect(Vec2D(0, 0), GetActualFrameSize());
}

Vec2D View::GetFrameSize() const {
  return {frame_.GetW(), frame_.GetH()};
}

Vec2D View::GetActualFrameSize() const {
  int a = padding_ * 2;
  return {frame_.GetW() - a, frame_.GetH() - a};
}

Vec2D View::GetFrameCoords() const {
  return {frame_.GetX(), frame_.GetY()};
}

Vec2D View::GetActualFrameCoords() const {
  return {frame_.GetX() + padding_, frame_.GetY() + padding_};
}

/*
void View::Render(Drawer* drawer) {
  if (!visible_) return;

  const Rect* frame = GetFrame();
  Rect actual_frame = *frame;

  drawer->SetDrawColor(bg_color_);
  drawer->FillRectAbs(frame);
  actual_frame.Contract(padding_);
  drawer->SetViewport(&actual_frame);

  Render(drawer);

  drawer->ResetViewport();
}
*/

bool View::RenderBegin(Drawer* drawer) {
  if (!visible_) return false;

  const Rect* frame = GetFrame();
  Rect actual_frame = *frame;

  drawer->SetDrawColor(bg_color_);
  drawer->FillRectAbs(frame);
  actual_frame.Contract(padding_);
  drawer->SetViewport(&actual_frame);

  return true;
}

void View::RenderEnd(Drawer* drawer) {
  drawer->ResetViewport();
}

bool View::DelegateMouseButtonEvent(MouseButtonEvent e) {
  if (visible_ && GetFrame()->Contains(e.GetCoords())) {
    Vec2D conv_coords = e.GetCoords() - GetActualFrameCoords();
    MouseButtonEvent ec(e.GetButton(), e.GetState(), conv_coords);
    if (OnMouseButtonEvent(ec)) return true;
  }
  return false;
}

bool View::DelegateMouseMotionEvent(MouseMotionEvent e) {
  if (!visible_) return false;
  if (e.IsMotionOver()) {
//    LOG_DEBUG("OnMouseOver (%d %d)", e.GetCoords().x, e.GetCoords().y);
    MouseMotionEvent ec;
    if (GetFrame()->Contains(e.GetCoords())) {
      Vec2D conv_coords = e.GetCoords() - GetActualFrameCoords();
      ec = MouseMotionEvent(MouseMotionEvent::kMotionOver, conv_coords, e.GetCoordsRel());
      if (OnMouseMotionEvent(ec)) return true;
    } else {
      if (GetFrame()->Contains(e.GetLastCoords())) {
        ec = MouseMotionEvent(MouseMotionEvent::kMotionOut, {0, 0}, {0, 0});
        OnMouseMotionEvent(ec);
      }
    }
  } else {
    ASSERT(e.IsMotionOut());
    OnMouseMotionEvent(e);
  }
  return false;
}

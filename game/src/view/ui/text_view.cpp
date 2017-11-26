#include "text_view.h"
#include "../drawer.h"

TextView::TextView(const Rect* frame, const std::string& text)
    : View(frame),
      text_(text),
      color_(COLOR_WHITE),
      size_(14),
      align_(LayoutHelper::kAlignLftTop) {
  SetBgColor(COLOR_TRANSPARENT);
  SetPadding(0);
}

void TextView::SetText(const std::string& text) {
  text_ = text;
}

void TextView::SetAlign(LayoutHelper::Align align) {
  align_ = align;
}

void TextView::SetColor(Color c) {
  color_ = c;
}

void TextView::RenderView(Drawer* drawer) {
  Rect frame = GetActualFrame();
  drawer->DrawTextAbs(text_, size_, color_, &frame, align_);
}

bool TextView::OnMouseButtonEvent(const MouseButtonEvent e) {
  if (e.IsLeftButtonDown()) {
    LOG_DEBUG("LeftButtonDown on the TextView '%s'", text_.c_str());
    return true;
  }
  return true;
}

bool TextView::OnMouseMotionEvent(const MouseMotionEvent e) {
  return false;
}


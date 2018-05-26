#include "composite_view.h"

namespace mengde {
namespace gui {
namespace uifw {

CompositeView::CompositeView() : View(), transparent_(false) {}

CompositeView::CompositeView(const Rect* rect) : View(rect), transparent_(false) {}

CompositeView::CompositeView(const Rect& rect) : View(rect), transparent_(false) {}

CompositeView::~CompositeView() {
  for (auto e : children_) {
    delete e;
  }
}

int CompositeView::AddChild(View* child) {
  children_.push_back(child);
  return (int)children_.size() - 1;
}

void CompositeView::RemoveChild(int index) {
  ASSERT(index >= 0 && index < (int)children_.size());
  auto child = children_.begin() + index;
  delete *child;
  children_.erase(child);
}

void CompositeView::RemoveChild(View* view) {
  auto child = std::find(children_.begin(), children_.end(), view);
  if (child != children_.end()) {
    delete *child;
    children_.erase(child);
    return;
  }

  LOG_ERROR("Given View object not found.");
  UNREACHABLE("Given View object not found.");
}

void CompositeView::RemoveAllChildren() {
  for (auto child : children_) {
    delete child;
  }
  children_.clear();
}

void CompositeView::ReplaceChild(View* existing_view, View* new_view) {
  auto child = std::find(children_.begin(), children_.end(), existing_view);
  if (child != children_.end()) {
    delete *child;
    *child = new_view;
  }
}

void CompositeView::Render(Drawer* drawer) {
  RENDER_BEGIN(this);
  for (auto child : children_) {
    child->Render(drawer);
  }
  RENDER_END(this);
}

void CompositeView::Update() {
  for (auto child : children_) {
    child->Update();
  }
}

bool CompositeView::OnMouseButtonEvent(const foundation::MouseButtonEvent& e) {
  // Mouse events should be handled in reverse order
  for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
    View* view = *itr;
    if (view->DelegateMouseButtonEvent(e)) return true;
  }
  return !transparent_;
}

bool CompositeView::OnMouseMotionEvent(const foundation::MouseMotionEvent& e) {
  if (e.IsMotionOver()) {
    bool is_handled = false;
    // Mouse events should be handled in reverse order
    for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
      View* view = *itr;
      is_handled = view->DelegateMouseMotionEvent(e) || is_handled;
      // Keep handling even if it is handled, since it may need to generate MotionOut event
    }
    return is_handled || !transparent_;
  } else {
    ASSERT(e.IsMotionOut());
    // Mouse events should be handled in reverse order
    for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
      View* view = *itr;
      view->DelegateMouseMotionEvent(e);
    }
    return true;
  }
}

bool CompositeView::OnMouseWheelEvent(const foundation::MouseWheelEvent& e) {
  // Mouse events should be handled in reverse order
  for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
    View* view = *itr;
    if (view->DelegateMouseWheelEvent(e)) return true;
  }
  return true;
}

bool CompositeView::OnKeyEvent(const foundation::KeyEvent& e) {
  for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
    View* view = *itr;
    if (view->DelegateKeyEvent(e)) return true;
  }
  return false;
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

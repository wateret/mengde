#include "composite_view.h"

CompositeView::CompositeView() : View() {
}

CompositeView::CompositeView(const Rect* rect) : View(rect) {
}

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
  children_.erase(children_.begin() + index);
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

bool CompositeView::OnMouseButtonEvent(const MouseButtonEvent e) {
  // Mouse events should be handled in reverse order
  for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
    View* view = *itr;
    if (view->DelegateMouseButtonEvent(e)) return true;
  }
  return true;
}

bool CompositeView::OnMouseMotionEvent(const MouseMotionEvent e) {
  if (e.IsMotionOver()) {
    bool is_handled = false;
    // Mouse events should be handled in reverse order
    for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
      View* view = *itr;
      is_handled = view->DelegateMouseMotionEvent(e) || is_handled;
    }
    return is_handled;
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

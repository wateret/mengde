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

void CompositeView::RenderView(Drawer* drawer) {
  for (auto itr = children_.begin(); itr != children_.end(); itr++) {
    View* view = *itr;
    view->Render(drawer);
  }
}

void CompositeView::Update() {
  for (auto v : children_) {
    v->Update();
  }
}

bool CompositeView::OnMouseButtonEvent(const MouseButtonEvent e) {
  for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
    View* view = *itr;
    if (view->DelegateMouseButtonEvent(e)) return true;
  }
  return true;
}

bool CompositeView::OnMouseMotionEvent(const MouseMotionEvent e) {
  if (e.IsMotionOver()) {
    bool is_handled = false;
    for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
      View* view = *itr;
      is_handled = view->DelegateMouseMotionEvent(e) || is_handled;
    }
    return is_handled;
  } else {
    ASSERT(e.IsMotionOut());
    for (auto itr = children_.rbegin(); itr != children_.rend(); itr++) {
      View* view = *itr;
      view->DelegateMouseMotionEvent(e);
    }
    return true;
  }
}

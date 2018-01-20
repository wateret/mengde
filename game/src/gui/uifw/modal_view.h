#ifndef MODAL_VIEW_H_
#define MODAL_VIEW_H_

#include "view_decorator.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class ModalView : public ViewDecorator {
 public:
  typedef std::function<bool(const foundation::MouseButtonEvent)> MouseButtonHandler;

 public:
  ModalView(const Rect& frame, View* view, const MouseButtonHandler&);
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override;

 private:
  MouseButtonHandler handler_;
};

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // MODAL_VIEW_H_

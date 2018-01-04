#ifndef UNIT_DIALOG_VIEW_H_
#define UNIT_DIALOG_VIEW_H_

#include "util/common.h"
#include "view/uifw/composite_view.h"

namespace mengde {
namespace core {
  class Unit;
}
}

class ImageView;
class TextView;

class UnitDialogView : public CompositeView {
 public:
  UnitDialogView(const Rect*, const string& = "", mengde::core::Unit* = nullptr);
  void SetText(const string&);
  void SetUnit(mengde::core::Unit* u);

 private:
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  string         message_;
  mengde::core::Unit*          unit_; // FIXME Do we really need this?
  CompositeView* frame_view_;
  ImageView*     iv_portrait_;
  TextView*      tv_name_;
  TextView*      tv_message_;
};

#endif // UNIT_DIALOG_VIEW_H_

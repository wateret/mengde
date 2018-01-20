#ifndef UNIT_DIALOG_VIEW_H_
#define UNIT_DIALOG_VIEW_H_

#include "util/common.h"
#include "gui/uifw/composite_view.h"

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

 private:
  string         message_;
  mengde::core::Unit*          unit_; // FIXME Do we really need this?
  ImageView*     iv_portrait_;
  TextView*      tv_name_;
  TextView*      tv_message_;
};

#endif // UNIT_DIALOG_VIEW_H_

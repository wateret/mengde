#ifndef MENGDE_GUI_APP_UNIT_DIALOG_VIEW_H_
#define MENGDE_GUI_APP_UNIT_DIALOG_VIEW_H_

#include "gui/uifw/composite_view.h"
#include "util/common.h"

#include "common.h"

namespace mengde {
namespace core {
class Unit;
}
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class UnitDialogView : public CompositeView {
 public:
  UnitDialogView(const Rect*, const string& = "", core::Unit* = nullptr);
  void SetText(const string&);
  void SetUnit(core::Unit* u);

 private:
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent&) override;

 private:
  string message_;
  core::Unit* unit_;  // FIXME Do we really need this?
  ImageView* iv_portrait_;
  TextView* tv_name_;
  TextView* tv_message_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_UNIT_DIALOG_VIEW_H_

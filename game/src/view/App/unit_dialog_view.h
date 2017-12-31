#ifndef UNIT_DIALOG_VIEW_H_
#define UNIT_DIALOG_VIEW_H_

#include "util/common.h"
#include "view/UIFW/composite_view.h"

class Unit;
class ImageView;
class TextView;

class UnitDialogView : public CompositeView {
 public:
  UnitDialogView(const Rect*, const string& = "", Unit* = nullptr);
  void SetText(const string&);
  void SetUnit(Unit* u);

 private:
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  string         message_;
  Unit*          unit_; // FIXME Do we really need this?
  CompositeView* frame_view_;
  ImageView*     iv_portrait_;
  TextView*      tv_name_;
  TextView*      tv_message_;
};

#endif // UNIT_DIALOG_VIEW_H_

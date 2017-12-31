#ifndef GAUGE_VIEW_H_
#define GAUGE_VIEW_H_

#include "view/foundation/view.h"
#include "view/foundation/rect.h"
#include "view/foundation/color.h"
#include "view/foundation/layout_helper.h"

class GaugeView : public View {
 public:
  enum HelpTextType {
    kHelpTextNone,
    kHelpTextCurMax,
    kHelpTextCurOnly
  };
 public:
  GaugeView(const Rect*, int, int, Color, Color);
  GaugeView(const Rect*, int, int, int, Color, Color, Color);
  virtual ~GaugeView();
  int GetCurVal() { return cur_val_; }
  int GetMaxVal() { return max_val_; }
  int GetExtVal() { return ext_val_; }
  void SetCurVal(int v) { cur_val_ = v; }
  void SetMaxVal(int v) { ASSERT(v > 0); max_val_ = v; }
  void SetExtVal(int v) { ext_val_ = v; }
  void SetHelpTextType(HelpTextType v) { help_text_type_ = v; }
  void SetHelpTextAlign(LayoutHelper::Align v) { help_text_align_ = v; }
  virtual void Render(Drawer*) override;

 private:
  int CalcWidth(int);

 private:
  Vec2D pos_;
  int   cur_val_;
  int   max_val_;
  int   ext_val_;
  Color cur_color_;
  Color max_color_;
  Color ext_color_;
  HelpTextType help_text_type_;
  LayoutHelper::Align help_text_align_;
};

#endif // GAUGE_VIEW_H_

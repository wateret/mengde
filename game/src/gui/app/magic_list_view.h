#ifndef MAGIC_LIST_VIEW_H_
#define MAGIC_LIST_VIEW_H_

#include "gui/uifw/composite_view.h"
#include "util/common.h"

#include "common.h"

namespace mengde {
namespace core {
class Game;
class Unit;
class MagicList;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class RootView;

class MagicListView : public CompositeView {
 public:
  static const int kTitleHeight = 24;

 public:
  MagicListView(const Rect&, core::Game* const, RootView* const);
  ~MagicListView();
  void         SetUnitAndMagicList(core::Unit*, shared_ptr<core::MagicList>);
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override;

 private:
  void Cleanup();

 private:
  core::Game* const game_;
  RootView* const   rv_;

 private:
  int item_height_;

 private:
  TextView*         tv_title_;
  VerticalListView* lv_magics_;
  View*             lv_magics_wrap_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MAGIC_LIST_VIEW_H_

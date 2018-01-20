#ifndef MAGIC_LIST_VIEW_H_
#define MAGIC_LIST_VIEW_H_

#include "gui/uifw/composite_view.h"
#include "util/common.h"

namespace mengde {
namespace gui {
namespace app {

namespace mengde {
namespace core {
  class Game;
  class Unit;
  class MagicList;
}
}

class RootView;
class TextView;
class VerticalListView;

class MagicListView : public CompositeView {
 public:
   static const int kTitleHeight = 24;

 public:
  MagicListView(const Rect&, mengde::core::Game* const, RootView* const);
  ~MagicListView();
  void SetUnitAndMagicList(mengde::core::Unit*, shared_ptr<mengde::core::MagicList>);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;

 private:
  void Cleanup();

 private:
  mengde::core::Game*     const game_;
  RootView* const rv_;

 private:
  int item_height_;

 private:
  TextView*         tv_title_;
  VerticalListView* lv_magics_;
  View*             lv_magics_wrap_;
};

} // namespace app
} // namespace gui
} // namespace mengde

#endif // MAGIC_LIST_VIEW_H_

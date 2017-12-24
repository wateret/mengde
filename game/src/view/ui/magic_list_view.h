#ifndef MAGIC_LIST_VIEW_H_
#define MAGIC_LIST_VIEW_H_

#include "composite_view.h"
#include "common.h"

class Game;
class RootView;
class Unit;
class MagicList;
class TextView;
class VerticalListView;

class MagicListView : public CompositeView {
 public:
   static const int kTitleHeight = 24;

 public:
  MagicListView(const Rect&, Game* const, RootView* const);
  ~MagicListView();
  void SetUnitAndMagicList(Unit*, shared_ptr<MagicList>);

 private:
  void Cleanup();

 private:
  Game*     const game_;
  RootView* const rv_;

 private:
  int item_height_;

 private:
  TextView*         tv_title_;
  VerticalListView* lv_magics_;
  View*             lv_magics_wrap_;
};

#endif // MAGIC_LIST_VIEW_H_

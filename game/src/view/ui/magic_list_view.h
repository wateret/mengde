#ifndef MAGIC_LIST_VIEW_H_
#define MAGIC_LIST_VIEW_H_

#include "callback_view.h"
#include "common.h"

class Unit;
class MagicList;
class Game;
class RootView;

class MagicListView : public CallbackView {
 public:
  MagicListView(const Rect*, Unit* = nullptr, MagicList* = nullptr);
  ~MagicListView();
  int GetHoverIndex() { return hover_index_; }
  void SetHoverIndex(int i) { hover_index_ = i; }
  void SetUnitAndMagicList(Unit*, MagicList*);
  void SetMouseUpHandler(Game* game, RootView* rv);
  int GetItemHeight() { return item_height_; }
  int NumItems();
  std::string GetHoverItem();

 private:
  void Cleanup();
  virtual void RenderView(Drawer*) override;

 private:
  Unit* unit_;
  MagicList* magic_list_;
  int item_height_;
  int hover_index_;
};

#endif // MAGIC_LIST_VIEW_H_

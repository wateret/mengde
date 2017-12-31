#ifndef CONTROL_VIEW_H_
#define CONTROL_VIEW_H_

#include "view/UIFW/composite_view.h"

class Game;
class RootView;
class TextView;

class ControlView : public CompositeView {
 public:
  ControlView(const Rect*, Game*, RootView*);
  void SetTurnText(int, int);

 private:
  Game* game_;
  RootView* rv_;
  TextView* tv_turn_;
};

#endif // CONTROL_VIEW_H_

#ifndef CONTROL_VIEW_H_
#define CONTROL_VIEW_H_

#include "composite_view.h"

class Game;
class RootView;

class ControlView : public CompositeView {
 public:
  ControlView(const Rect*, Game*, RootView*);

 private:
  Game* game_;
  RootView* rv_;
};

#endif // CONTROL_VIEW_H_

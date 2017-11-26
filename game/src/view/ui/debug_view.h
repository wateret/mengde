#ifndef DEBUG_VIEW_H_
#define DEBUG_VIEW_H_

#include "composite_view.h"

class Game;
class RootView;

class DebugView : public CompositeView {
 public:
  DebugView(const Rect*, Game*, RootView*);

 private:
  Game* game_;
  RootView* rv_;
};

#endif // DEBUG_VIEW_H_

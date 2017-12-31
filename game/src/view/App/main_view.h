#ifndef MAIN_VIEW_H_
#define MAIN_VIEW_H_

#include "view/UIFW/composite_view.h"

class App;

class MainView : public CompositeView {
 public:
  MainView(const Rect*, App*);

 private:
};

#endif // MAIN_VIEW_H_

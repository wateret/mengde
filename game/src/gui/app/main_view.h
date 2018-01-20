#ifndef MAIN_VIEW_H_
#define MAIN_VIEW_H_

#include "gui/uifw/composite_view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace app {

class App;

class MainView : public CompositeView {
 public:
  MainView(const Rect*, App*);

 private:
};

} // namespace app
} // namespace gui
} // namespace mengde

#endif // MAIN_VIEW_H_

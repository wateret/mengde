#ifndef TAB_VIEW_H_
#define TAB_VIEW_H_

#include "common.h"
#include "composite_view.h"

class TabView : public CompositeView {
 public:
  TabView(const Rect*);
  void AddTab(const string&, View*);
  int GetNumTabs() { return (int)v_tab_buttons_.size(); }
  void SetViewIndex(int);

 private:
  std::vector<View*> v_tab_buttons_;
  std::vector<View*> v_tabs_;
  int                view_index_;
};

#endif // TAB_VIEW_H_

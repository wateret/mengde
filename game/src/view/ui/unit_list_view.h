#ifndef UNIT_LIST_VIEW_H_
#define UNIT_LIST_VIEW_H_

#include "composite_view.h"

class Unit;
class VerticalListView;

class UnitListView : public CompositeView {
 public:
  UnitListView(const Rect&, const vector<Unit*>&);

 private:
  VerticalListView* list_view_;
  vector<Unit*> unit_list_;
};

#endif // UNIT_LIST_VIEW_H_

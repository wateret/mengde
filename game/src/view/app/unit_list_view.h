#ifndef UNIT_LIST_VIEW_H_
#define UNIT_LIST_VIEW_H_

#include "view/uifw/composite_view.h"

class Unit;
class VerticalListView;
class TextView;

class UnitListView : public CompositeView {
 public:
  class UnitDetailView : public CompositeView {
   public:
    UnitDetailView(const Rect&);
    void SetUnit(Unit*);

   private:
    Unit*     unit_;
    TextView* tv_name_;
  };

 public:
  UnitListView(const Rect&, const vector<Unit*>&);
  void SetUnit(Unit* unit) { unit_detail_view_->SetUnit(unit); }

 private:
  vector<Unit*> unit_list_;
  UnitDetailView* unit_detail_view_;
};

#endif // UNIT_LIST_VIEW_H_

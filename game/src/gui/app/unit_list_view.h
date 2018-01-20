#ifndef UNIT_LIST_VIEW_H_
#define UNIT_LIST_VIEW_H_

#include "gui/uifw/composite_view.h"

namespace mengde {
namespace gui {
namespace app {

namespace mengde {
namespace core {
  class Unit;
}
}

class VerticalListView;
class TextView;

class UnitListView : public CompositeView {
 public:
  class UnitDetailView : public CompositeView {
   public:
    UnitDetailView(const Rect&);
    void SetUnit(mengde::core::Unit*);

   private:
    mengde::core::Unit*     unit_;
    TextView* tv_name_;
  };

 public:
  UnitListView(const Rect&, const vector<mengde::core::Unit*>&);
  void SetUnit(mengde::core::Unit* unit) { unit_detail_view_->SetUnit(unit); }

 private:
  vector<mengde::core::Unit*> unit_list_;
  UnitDetailView* unit_detail_view_;
};

} // namespace app
} // namespace gui
} // namespace mengde

#endif // UNIT_LIST_VIEW_H_

#ifndef EQUIPMENT_SELECT_VIEW_H_
#define EQUIPMENT_SELECT_VIEW_H_

#include "view/uifw/callback_view.h"
#include "view/uifw/composite_view.h"

class Equipment;
class RowMajorListView;

class ItemIconView : public CallbackView {
 public:
  ItemIconView(const Rect&, const string&);
};

class EquipmentSelectView : public CompositeView {
 public:
  EquipmentSelectView(const Rect&);
  void SetEquipments(const vector<const Equipment*>&);

 private:
  RowMajorListView* equipment_list_view_;
};

#endif // EQUIPMENT_SELECT_VIEW_H_

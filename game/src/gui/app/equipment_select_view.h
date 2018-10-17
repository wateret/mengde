#ifndef MENGDE_GUI_APP_EQUIPMENT_SELECT_VIEW_H_
#define MENGDE_GUI_APP_EQUIPMENT_SELECT_VIEW_H_

#include "core/assets.h"  // XXX temporary include for EquipmentWithAmount
#include "gui/uifw/callback_view.h"
#include "gui/uifw/composite_view.h"

#include "common.h"

namespace mengde {
namespace core {
class Assets;
class Hero;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class EquipmentSetView;
class GameView;

class ItemIconView : public CallbackView {
 public:
  ItemIconView(const Rect&, GameView*, const core::EquipmentWithAmount&);

 private:
  GameView* gv_;
};

class EquipmentSelectView : public CompositeView {
 public:
  EquipmentSelectView(const Rect&, GameView*, EquipmentSetView*);
  void SetHero(const core::Hero* hero) { hero_ = hero; }
  void SetEquipments(const vector<core::EquipmentWithAmount>&, core::Assets*);

 private:
  GameView* gv_;
  const core::Hero* hero_;
  RowMajorListView* equipment_list_view_;
  EquipmentSetView* equipment_set_view_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_EQUIPMENT_SELECT_VIEW_H_

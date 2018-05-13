#ifndef MENGDE_GUI_APP_UI_VIEWS_H_
#define MENGDE_GUI_APP_UI_VIEWS_H_

#include "common.h"
#include "gui/uifw/composite_view.h"

namespace mengde {
namespace core {

class Scenario;

}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class GameView;
class DeployView;
class UnitTooltipView;
class UnitView;
class ControlView;
class UnitDialogView;
class MagicListView;
class TerrainInfoView;
class UnitListView;
class UnitActionView;

class UIViews : public CompositeView {
 public:
  UIViews(const Rect& rect, core::Scenario*, GameView*);

 public:
  // UI view getters
  TerrainInfoView* terrain_info_view() { return terrain_info_view_; }
  UnitListView* unit_list_view() { return unit_list_view_; }
  UnitDialogView* unit_dialog_view() { return unit_dialog_view_; }
  ModalView* unit_dialog_view_wrapper() { return unit_dialog_view_wrapper_; }
  UnitView* unit_view() { return unit_view_; }
  UnitTooltipView* unit_tooltip_view() { return unit_tooltip_view_; }
  ControlView* control_view() { return control_view_; }
  ModalDialogView* dialog_view() { return dialog_view_; }
  MagicListView* magic_list_view() { return magic_list_view_; }
  UnitActionView* unit_action_view() { return unit_action_view_; }

 private:
  DeployView* deploy_view_;
  UnitTooltipView* unit_tooltip_view_;
  UnitView* unit_view_;
  ControlView* control_view_;
  ModalDialogView* dialog_view_;
  UnitDialogView* unit_dialog_view_;
  ModalView* unit_dialog_view_wrapper_;
  MagicListView* magic_list_view_;
  TerrainInfoView* terrain_info_view_;
  UnitListView* unit_list_view_;
  UnitActionView* unit_action_view_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_UI_VIEWS_H_

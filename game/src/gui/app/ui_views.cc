#include "ui_views.h"

#include "control_view.h"
#include "core/game.h"
#include "core/scenario.h"
#include "deploy_view.h"
#include "game_view.h"
#include "gui/uifw/layout_helper.h"
#include "gui/uifw/modal_dialog_view.h"
#include "gui/uifw/modal_view.h"
#include "gui/uifw/scroll_view.h"
#include "magic_list_view.h"
#include "terrain_info_view.h"
#include "unit_action_view.h"
#include "unit_dialog_view.h"
#include "unit_list_view.h"
#include "unit_tooltip_view.h"
#include "unit_view.h"

namespace mengde {
namespace gui {
namespace app {

UIViews::UIViews(const Rect& rect, core::Scenario* scenario, GameView* game_view) : CompositeView(rect) {
  core::Game* game = scenario->GetGame();

  {  // Initialize deploy_view_
    core::Assets* assets = scenario->GetAssets();
    Rect frame = LayoutHelper::CalcPosition(GetFrameSize(), {680, 480}, LayoutHelper::kAlignCenter);

    deploy_view_ = new DeployView(frame, assets, game, game_view);
    deploy_view_->visible(true);
    AddChild(deploy_view_);
  }

  // Create UI views

  Rect unit_frame =
      LayoutHelper::CalcPosition(GetFrameSize(), {220, 320}, LayoutHelper::kAlignRgtTop, LayoutHelper::kDefaultSpace);
  unit_view_ = new UnitView(unit_frame);
  //  unit_view_->visible(false);
  AddChild(unit_view_);

  {  // Initialize control_view_
    Rect control_frame =
        LayoutHelper::CalcPosition(GetFrameSize(), {300, 136}, LayoutHelper::kAlignRgtBot, LayoutHelper::kDefaultSpace);
    control_view_ = new ControlView(control_frame, game, game_view);
    AddChild(control_view_);
  }

  {  // Initialize dialog_view_
    Rect dialog_frame = GetFrame();
    dialog_view_ = new ModalDialogView(dialog_frame, "Put your message here!");
    dialog_view_->visible(false);
    AddChild(dialog_view_);
  }

  {  // Initialize magic_list_view_;
    Rect magic_list_frame = {8, 8, 170, 200};
    magic_list_view_ = new MagicListView(magic_list_frame, game, game->user_interface(), game_view);
    magic_list_view_->visible(false);
    AddChild(magic_list_view_);
  }

  {  // Initalize unit_tooltip_view_
    Rect unit_info_frame =
        LayoutHelper::CalcPosition(GetFrameSize(), {200, 100}, LayoutHelper::kAlignLftBot, LayoutHelper::kDefaultSpace);
    unit_tooltip_view_ = new UnitTooltipView(unit_info_frame);
    AddChild(unit_tooltip_view_);
  }

  {  // Initialize unit_dialog_view_
    Rect unit_dialog_frame =
        LayoutHelper::CalcPosition(GetFrameSize(), {360, 120}, LayoutHelper::kAlignCenter, LayoutHelper::kDefaultSpace);
    unit_dialog_view_ = new UnitDialogView(unit_dialog_frame);
    unit_dialog_view_wrapper_ =
        new ModalView(GetActualFrame(), unit_dialog_view_, [=](const foundation::MouseButtonEvent e) {
          if (e.IsLeftButtonUp() || e.IsRightButtonUp()) {
            unit_dialog_view_wrapper_->visible(false);
          }
          return true;
        });
    unit_dialog_view_wrapper_->visible(false);
    AddChild(unit_dialog_view_wrapper_);
  }

  {  // Initialize terrain_info_view_
    Rect terrain_frame =
        LayoutHelper::CalcPosition(GetFrameSize(), {200, 100}, LayoutHelper::kAlignLftBot, LayoutHelper::kDefaultSpace);
    terrain_info_view_ = new TerrainInfoView(terrain_frame, {0, 0});  // FIXME temporary coords
    terrain_info_view_->visible(false);
    AddChild(terrain_info_view_);
  }

  // TODO Enable UnitListView
#if 0
  {  // Initialize unit_list_view_
    Rect frame = LayoutHelper::CalcPosition(GetFrameSize(), {680, 480}, LayoutHelper::kAlignCenter);
    unit_list_view_ = new UnitListView(frame, game->GetCurrentTurnUnits());
    unit_list_view_->visible(false);
    AddChild(unit_list_view_);
  }
#endif

  {  // Initialize unit_action_view_
    Rect frame = LayoutHelper::CalcPosition(GetFrameSize(), {150, 150}, LayoutHelper::kAlignLftTop);
    unit_action_view_ = new UnitActionView(frame, game, game->user_interface(), game_view);
    unit_action_view_->visible(false);
    AddChild(unit_action_view_);
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

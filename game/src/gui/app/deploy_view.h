#ifndef MENGDE_GUI_APP_DEPLOY_VIEW_H_
#define MENGDE_GUI_APP_DEPLOY_VIEW_H_

#include "gui/uifw/callback_view.h"
#include "gui/uifw/composite_view.h"

#include "common.h"

namespace mengde {
namespace core {

class Hero;
class Assets;
class IDeployHelper;

}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class EquipmentSetView;
class EquipmentSelectView;
class GameView;
class UnitOverView;
class UnitView;
class DeployDirector;

class HeroModelView : public CallbackView {
 public:
  HeroModelView(const Rect&, const core::Hero*, core::IDeployHelper*);
  void UpdateViews();
  void SetDeployNo(uint32_t no) { deploy_no_ = no; }
  bool IsSelected() { return deploy_no_ != 0; }

 private:
  const core::Hero* hero_;
  uint32_t          deploy_no_;
  bool              required_unselectable_;
  TextView*         tv_no_;
};

class HeroModelListView : public CompositeView {
 public:
  HeroModelListView(const Rect&, const vector<const core::Hero*>&, core::IDeployHelper*, DeployDirector*);
};

// DeployView is a UI view for deploying heroes

class DeployView : public CompositeView {
 public:
  DeployView(const Rect&, core::Assets*, core::IDeployHelper*, GameView*);

 private:
  GameView*            gv_;
  EquipmentSelectView* equipment_select_view_;
  DeployDirector*      director_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_DEPLOY_VIEW_H_

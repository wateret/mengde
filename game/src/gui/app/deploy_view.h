#ifndef DEPLOY_VIEW_H_
#define DEPLOY_VIEW_H_

#include "gui/uifw/composite_view.h"
#include "gui/uifw/callback_view.h"
#include "i_equipment_set_setter.h"

namespace mengde {
namespace core {
  class Hero;
  class Assets;
  class IDeployHelper;
}
}

class TextView;
class EquipmentSetView;
class EquipmentSelectView;

class HeroModelView : public CallbackView {
 public:
  HeroModelView(const Rect&, shared_ptr<const mengde::core::Hero>, mengde::core::IDeployHelper*, IEquipmentSetSetter*);
  void UpdateViews();
  void SetDeployNo(uint32_t no) { deploy_no_ = no; }
  bool IsSelected() { return deploy_no_ != 0; }

 private:
  shared_ptr<const mengde::core::Hero> hero_;
  uint32_t deploy_no_;
  bool     required_unselectable_;
  TextView* tv_no_;
};

class HeroModelListView : public CompositeView {
 public:
  HeroModelListView(const Rect&, const vector<shared_ptr<const mengde::core::Hero>>&, mengde::core::IDeployHelper*,
                    IEquipmentSetSetter*, EquipmentSelectView*);
};

// DeployView is a UI view for deploying heroes

class DeployView : public CompositeView {
 public:
  DeployView(const Rect&, mengde::core::Assets*, mengde::core::IDeployHelper*);

 private:
  EquipmentSetView* equipment_set_view_;
  EquipmentSelectView* equipment_select_view_;
};

#endif // DEPLOY_VIEW_H_

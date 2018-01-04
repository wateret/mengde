#ifndef DEPLOY_VIEW_H_
#define DEPLOY_VIEW_H_

#include "view/uifw/composite_view.h"
#include "view/uifw/callback_view.h"
#include "i_equipment_set_setter.h"

namespace mengde {
namespace core {
  class Hero;
  class IDeployHelper;
}
}

class TextView;
class EquipmentSetView;
class EquipmentSelectView;

class HeroModelView : public CallbackView {
 public:
  HeroModelView(const Rect&, shared_ptr<mengde::core::Hero>, mengde::core::IDeployHelper*, IEquipmentSetSetter*);
  void UpdateViews();
  bool IsSelected() { return deploy_no_ != 0; }

 private:
  shared_ptr<mengde::core::Hero> hero_;
  uint32_t deploy_no_;
  bool     required_unselectable_;
  TextView* tv_no_;
};

class HeroModelListView : public CompositeView {
 public:
  HeroModelListView(const Rect&, const vector<shared_ptr<mengde::core::Hero>>&, mengde::core::IDeployHelper*, IEquipmentSetSetter*);
  vector<shared_ptr<mengde::core::Hero>> GetHeroesSelected() { return heroes_selected_; }

 private:
  vector<shared_ptr<mengde::core::Hero>> heroes_selected_;
};

// DeployView is a UI view for deploying heroes

class DeployView : public CompositeView {
 public:
  DeployView(const Rect&, const vector<shared_ptr<mengde::core::Hero>>&, mengde::core::IDeployHelper*);

 private:
  vector<shared_ptr<mengde::core::Hero>> hero_list_;
  EquipmentSetView* equipment_set_view_;
  EquipmentSelectView* equipment_select_view_;
};

#endif // DEPLOY_VIEW_H_

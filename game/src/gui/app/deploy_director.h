#ifndef MENGDE_GUI_APP_DEPLOY_DIRECTOR_H_
#define MENGDE_GUI_APP_DEPLOY_DIRECTOR_H_

namespace mengde {
namespace core {

class Hero;

}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class EquipmentSelectView;
class HeroModelListView;
class UnitView;
class HeroModelView;

class DeployDirector {
 public:
  DeployDirector() = default;
  void Init(HeroModelListView*, UnitView*, EquipmentSelectView*);
  void OnDeployNoChanged(HeroModelView* model_view, int deploy_no);
  void OnHeroChosen(const core::Hero* hero);

 private:
  HeroModelListView* hero_model_list_view_;
  UnitView* unit_view_;
  EquipmentSelectView* equipment_select_view_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_DEPLOY_DIRECTOR_H_

#include "deploy_director.h"

#include "core/hero.h"
#include "deploy_view.h"
#include "equipment_select_view.h"
#include "unit_view.h"

namespace mengde {
namespace gui {
namespace app {

void DeployDirector::Init(HeroModelListView* hero_model_list_view, UnitView* unit_view,
                          EquipmentSelectView* equipment_select_view) {
  hero_model_list_view_ = hero_model_list_view;
  unit_view_ = unit_view;
  equipment_select_view_ = equipment_select_view;
}

void DeployDirector::OnDeployNoChanged(HeroModelView* model_view, int deploy_no) {
  model_view->SetDeployNo(deploy_no);
  model_view->UpdateViews();
}

void DeployDirector::OnHeroChosen(const core::Hero* hero) {
  equipment_select_view_->SetHero(hero);
  unit_view_->SetUnit(hero);
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

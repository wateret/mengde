#include "deploy_view.h"

#include <algorithm>

#include "core/assets.h"
#include "core/equipment.h"
#include "core/force.h"
#include "core/hero.h"
#include "core/i_deploy_helper.h"
#include "core/unit.h"
#include "deploy_director.h"
#include "equipment_select_view.h"
#include "equipment_set_view.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/image_view.h"
#include "gui/uifw/text_view.h"
#include "resource_path.h"
#include "unit_over_view.h"
#include "unit_view.h"

namespace mengde {
namespace gui {
namespace app {

HeroModelView::HeroModelView(const Rect& frame, const core::Hero* hero, core::IDeployHelper* deploy_helper)
    : CallbackView(frame), hero_(hero), deploy_no_(0), required_unselectable_(false), tv_no_(nullptr) {
  padding(4);
  Rect img_src_rect(0, 0, 48, 48);
  Rect iv_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), img_src_rect.GetSize(), LayoutHelper::kAlignCenter);
  iv_frame.SetY(iv_frame.GetY() - 8);
  ImageView* iv_hero = new ImageView(iv_frame, rcpath::UnitModelPath(hero->GetModelId(), kSpriteStand).ToString());
  iv_hero->SetSourceRect(img_src_rect);
  AddChild(iv_hero);

  Rect      tv_hero_frame = GetActualFrame();
  TextView* tv_hero = new TextView(&tv_hero_frame, hero_->GetId(), COLOR("white"), 14, LayoutHelper::kAlignMidBot);
  AddChild(tv_hero);

  Rect tv_no_frame = GetActualFrame();
  tv_no_frame.SetW(tv_no_frame.GetW() - 8);
  tv_no_ = new TextView(&tv_no_frame, "", COLOR("orange"), 20, LayoutHelper::kAlignRgtTop);
  AddChild(tv_no_);

  deploy_no_             = deploy_helper->FindDeploy(hero);
  required_unselectable_ = (deploy_no_ != 0);  // Assume unselectable if already deployed at this point(construction)
  UpdateViews();
}

void HeroModelView::UpdateViews() {
  if (required_unselectable_) {
    ASSERT(IsSelected());
    bg_color(COLOR("black"));
    tv_no_->SetText("");
  } else if (IsSelected()) {
    bg_color(COLOR("gray"));
    tv_no_->SetText(std::to_string(deploy_no_));
  } else {
    bg_color(COLOR("transparent"));
    tv_no_->SetText("");
  }
}

HeroModelListView::HeroModelListView(const Rect& frame, const vector<const core::Hero*>& hero_list,
                                     core::IDeployHelper* deploy_helper, DeployDirector* director)
    : CompositeView(frame) {
  bg_color(COLOR("navy"));
  static const Vec2D kHeroModelSize = {96, 80};
  Rect               hero_model_frame({0, 0}, kHeroModelSize);
  for (auto hero : hero_list) {
    HeroModelView* model_view = new HeroModelView(hero_model_frame, hero, deploy_helper);
    model_view->SetMouseButtonHandler(
        [model_view, hero, deploy_helper, director](const foundation::MouseButtonEvent e) -> bool {
          if (e.IsLeftButtonUp()) {
            int deploy_no = 0;
            if (model_view->IsSelected()) {
              deploy_no = deploy_helper->UnassignDeploy(hero);
            } else {
              deploy_no = deploy_helper->AssignDeploy(hero);
            }
            director->OnDeployNoChanged(model_view, deploy_no);
            director->OnHeroChosen(hero);
          }
          return true;
        });
    AddChild(model_view);
    hero_model_frame.SetX(hero_model_frame.GetX() + kHeroModelSize.x);
    if (hero_model_frame.GetRight() > GetActualFrameSize().x) {
      hero_model_frame.SetX(0);
      hero_model_frame.SetY(hero_model_frame.GetY() + kHeroModelSize.y);
    }
  }
}

DeployView::DeployView(const Rect& frame, core::Assets* assets, core::IDeployHelper* deploy_helper)
    : CompositeView(frame) {
  padding(8);
  bg_color(COLOR("darkgray"));

  director_ = new DeployDirector();

  Rect      unit_view_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {204, 320}, LayoutHelper::kAlignRgtTop);
  UnitView* unit_view       = new UnitView(unit_view_frame);
  unit_view->padding(0);
  AddChild(unit_view);

  EquipmentSetView* equipment_set_view = unit_view->equipment_set_view();

  Rect equipment_select_frame = GetActualFrame();
  equipment_select_frame.SetW(4 * 96);
  equipment_select_view_ = new EquipmentSelectView(equipment_select_frame, equipment_set_view);
  equipment_select_view_->visible(false);

  {  // Initialize equipment_set_view
    EquipmentSelectView* select_view = equipment_select_view_;

    auto mouse_handler_gen = [select_view, assets](core::Equipment::Type type) {
      return [select_view, assets, type](const foundation::MouseButtonEvent e) {
        if (e.IsLeftButtonUp()) {
          if (select_view->visible()) {
            select_view->visible(false);
          } else {
            vector<core::EquipmentWithAmount> equipments = assets->GetEquipmentsWithAmount();
            vector<core::EquipmentWithAmount> equipments_filtered;
            std::copy_if(equipments.begin(), equipments.end(), std::back_inserter(equipments_filtered),
                         [type](const core::EquipmentWithAmount& eq) { return eq.object->GetType() == type; });
            select_view->SetEquipments(equipments_filtered, assets);

            select_view->visible(true);
          }
        }
        return true;
      };
    };
    equipment_set_view->SetWeaponMouseButtonHandler(mouse_handler_gen(core::Equipment::Type::kWeapon));
    equipment_set_view->SetArmorMouseButtonHandler(mouse_handler_gen(core::Equipment::Type::kArmor));
    equipment_set_view->SetAidMouseButtonHandler(mouse_handler_gen(core::Equipment::Type::kAid));
  }

  HeroModelListView* hero_model_list_view;
  {
    Rect hero_model_list_frame = GetActualFrame();
    hero_model_list_frame.SetW(4 * 96);
    hero_model_list_view = new HeroModelListView(hero_model_list_frame, assets->GetHeroes(), deploy_helper, director_);
    AddChild(hero_model_list_view);
  }

  Rect        btn_ok_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {100, 50}, LayoutHelper::kAlignRgtBot);
  ButtonView* btn_ok       = new ButtonView(&btn_ok_frame, "To Battle");
  btn_ok->SetMouseButtonHandler([&, deploy_helper](foundation::MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      if (deploy_helper->SubmitDeploy()) {
        this->visible(false);
      }
    }
    return true;
  });

  AddChild(equipment_select_view_);
  AddChild(btn_ok);

  director_->Init(hero_model_list_view, unit_view->unit_over_view(), equipment_set_view, equipment_select_view_);
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

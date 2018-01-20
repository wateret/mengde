#include "deploy_view.h"

#include <algorithm>

#include "core/equipment.h"
#include "core/assets.h"
#include "core/hero.h"
#include "core/i_deploy_helper.h"
#include "gui/uifw/button_view.h"
#include "gui/uifw/image_view.h"
#include "gui/uifw/text_view.h"
#include "misc.h"
#include "equipment_set_view.h"
#include "equipment_select_view.h"

namespace mengde {
namespace gui {
namespace app {

HeroModelView::HeroModelView(const Rect& frame,
                             shared_ptr<const mengde::core::Hero> hero,
                             mengde::core::IDeployHelper* deploy_helper,
                             IEquipmentSetSetter* equipment_set_setter)
    : CallbackView(frame), hero_(hero), deploy_no_(0), required_unselectable_(false), tv_no_(nullptr) {
  padding(4);
  Rect img_src_rect(0, 0, 48, 48);
  Rect iv_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), img_src_rect.GetSize(), LayoutHelper::kAlignCenter);
  iv_frame.SetY(iv_frame.GetY() - 8);
  ImageView* iv_hero = new ImageView(iv_frame, GetModelPath(hero->GetBitmapPath(), kSpriteStand));
  iv_hero->SetSourceRect(img_src_rect);
  AddChild(iv_hero);

  Rect tv_hero_frame = GetActualFrame();
  TextView* tv_hero = new TextView(&tv_hero_frame, hero_->GetId(), COLOR("white"), 14, LayoutHelper::kAlignMidBot);
  AddChild(tv_hero);

  Rect tv_no_frame = GetActualFrame();
  tv_no_frame.SetW(tv_no_frame.GetW() - 8);
  tv_no_ = new TextView(&tv_no_frame, "", COLOR("orange"), 20, LayoutHelper::kAlignRgtTop);
  AddChild(tv_no_);

  deploy_no_ = deploy_helper->FindDeploy(hero);
  required_unselectable_ = (deploy_no_ != 0); // Assume unselectable if already deployed at this point(construction)
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

HeroModelListView::HeroModelListView(const Rect& frame,
                                     const vector<shared_ptr<const mengde::core::Hero>>& hero_list,
                                     mengde::core::IDeployHelper* deploy_helper,
                                     IEquipmentSetSetter* equipment_set_setter,
                                     EquipmentSelectView* equipment_select_view)
    : CompositeView(frame) {
  bg_color(COLOR("navy"));
  static const Vec2D kHeroModelSize = {96, 80};
  Rect hero_model_frame({0, 0}, kHeroModelSize);
  for (auto hero : hero_list) {
    HeroModelView* model_view = new HeroModelView(hero_model_frame, hero, deploy_helper, equipment_set_setter);
    model_view->SetMouseButtonHandler([this, model_view, hero, deploy_helper, equipment_set_setter, equipment_select_view] (const MouseButtonEvent e) -> bool {
      if (e.IsLeftButtonUp()) {
        if (model_view->IsSelected()) {
          model_view->SetDeployNo(deploy_helper->UnassignDeploy(hero));
        } else {
          model_view->SetDeployNo(deploy_helper->AssignDeploy(hero));
        }
        equipment_select_view->SetHero(hero);
        equipment_set_setter->SetEquipmentSet(hero->GetEquipmentSet());
        model_view->UpdateViews();
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

DeployView::DeployView(const Rect& frame, mengde::core::Assets* assets, mengde::core::IDeployHelper* deploy_helper)
    : CompositeView(frame) {
  padding(8);
  bg_color(COLOR("darkgray"));

  Rect equipment_set_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {220, 270}, LayoutHelper::kAlignRgtTop);
  equipment_set_view_ = new EquipmentSetView(&equipment_set_frame);
  Rect equipment_select_frame = GetActualFrame();
  equipment_select_frame.SetW(4 * 96);
  equipment_select_view_ = new EquipmentSelectView(equipment_select_frame, equipment_set_view_);

  { // Initialize equipment_select_view_
    equipment_select_view_->visible(false);
  }

  { // Initialize equipment_set_view_
    EquipmentSelectView* select_view = equipment_select_view_;
    equipment_set_view_->bg_color(COLOR("navy"));
    equipment_set_view_->padding(8);

    auto mouse_handler_gen = [select_view, assets] (mengde::core::Equipment::Type type) {
      return [select_view, assets, type] (const MouseButtonEvent e) {
        if (e.IsLeftButtonUp()) {
          if (select_view->visible()) {
            select_view->visible(false);
          } else {
            vector<mengde::core::EquipmentWithAmount> equipments = assets->GetEquipmentsWithAmount();
            vector<mengde::core::EquipmentWithAmount> equipments_filtered;
            std::copy_if(equipments.begin(), equipments.end(), std::back_inserter(equipments_filtered),
                         [type] (const mengde::core::EquipmentWithAmount& eq) { return eq.object->GetType() == type; });
            select_view->SetEquipments(equipments_filtered, assets);

            select_view->visible(true);
          }
        }
        return true;
      };
    };
    equipment_set_view_->SetWeaponMouseButtonHandler(mouse_handler_gen(mengde::core::Equipment::Type::kWeapon));
    equipment_set_view_->SetArmorMouseButtonHandler(mouse_handler_gen(mengde::core::Equipment::Type::kArmor));
    equipment_set_view_->SetAidMouseButtonHandler(mouse_handler_gen(mengde::core::Equipment::Type::kAid));
    AddChild(equipment_set_view_);
  }

  Rect hero_model_list_frame = GetActualFrame();
  hero_model_list_frame.SetW(4 * 96);
  HeroModelListView* hero_model_list_view =
      new HeroModelListView(hero_model_list_frame, assets->GetHeroes(), deploy_helper, equipment_set_view_, equipment_select_view_);
  AddChild(hero_model_list_view);

  Rect btn_ok_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {100, 50}, LayoutHelper::kAlignRgtBot);
  ButtonView* btn_ok = new ButtonView(&btn_ok_frame, "To Battle");
  btn_ok->SetMouseButtonHandler([this, deploy_helper] (MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      if (deploy_helper->SubmitDeploy()) {
        this->visible(false);
      }
    }
    return true;
  });
  AddChild(btn_ok);

  AddChild(equipment_select_view_);
}

} // namespace app
} // namespace gui
} // namespace mengde

#include "deploy_view.h"
#include "core/hero.h"
#include "core/i_deploy_helper.h"
#include "view/UIFW/button_view.h"
#include "view/UIFW/image_view.h"
#include "view/UIFW/text_view.h"
#include "view/Foundation/misc.h"

HeroModelView::HeroModelView(const Rect& frame, shared_ptr<Hero> hero, IDeployHelper* deploy_helper)
    : CallbackView(frame), hero_(hero), deploy_no_(0), required_unselectable_(false), tv_no_(nullptr) {
  SetPadding(4);
  Rect img_src_rect(0, 0, 48, 48);
  Rect iv_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), img_src_rect.GetSize(), LayoutHelper::kAlignCenter);
  iv_frame.SetY(iv_frame.GetY() - 8);
  ImageView* iv_hero = new ImageView(iv_frame, Misc::GetModelPath(hero->GetBitmapPath(), kSpriteStand));
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

  SetMouseButtonHandler([this, hero, deploy_helper] (MouseButtonEvent e) -> bool {
    if (e.IsLeftButtonUp()) {
      if (IsSelected()) {
        deploy_no_ = deploy_helper->UnassignDeploy(hero);
      } else {
        deploy_no_ = deploy_helper->AssignDeploy(hero);
      }
      UpdateViews();
    }
    return true;
  });
}

void HeroModelView::UpdateViews() {
  if (required_unselectable_) {
    ASSERT(IsSelected());
    SetBgColor(COLOR("black"));
    tv_no_->SetText("");
  } else if (IsSelected()) {
    SetBgColor(COLOR("gray"));
    tv_no_->SetText(std::to_string(deploy_no_));
  } else {
    SetBgColor(COLOR("transparent"));
    tv_no_->SetText("");
  }
}

HeroModelListView::HeroModelListView(const Rect& frame, const vector<shared_ptr<Hero>>& hero_list, IDeployHelper* deploy_helper)
    : CompositeView(frame) {
  SetBgColor(COLOR("navy"));
  static const Vec2D kHeroModelSize = {96, 80};
  Rect hero_model_frame({0, 0}, kHeroModelSize);
  for (auto hero : hero_list) {
    HeroModelView* hero_model_view = new HeroModelView(hero_model_frame, hero, deploy_helper);
    AddChild(hero_model_view);
    hero_model_frame.SetX(hero_model_frame.GetX() + kHeroModelSize.x);
    if (hero_model_frame.GetRight() > GetActualFrameSize().x) {
      hero_model_frame.SetX(0);
      hero_model_frame.SetY(hero_model_frame.GetY() + kHeroModelSize.y);
    }
  }
}

DeployView::DeployView(const Rect& frame, const vector<shared_ptr<Hero>>& hero_list, IDeployHelper* deploy_helper)
    : CompositeView(frame), hero_list_(hero_list) {
  SetPadding(8);
  SetBgColor(COLOR("darkgray"));
  Rect hero_model_list_frame = GetActualFrame();
  hero_model_list_frame.SetW(5 * 96);
  HeroModelListView* hero_model_list_view = new HeroModelListView(hero_model_list_frame, hero_list_, deploy_helper);
  AddChild(hero_model_list_view);

  Rect btn_ok_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {100, 50}, LayoutHelper::kAlignRgtBot);
  ButtonView* btn_ok = new ButtonView(&btn_ok_frame, "To Battle");
  btn_ok->SetMouseButtonHandler([this, deploy_helper] (MouseButtonEvent e) {
    if (e.IsLeftButtonUp()) {
      if (deploy_helper->SubmitDeploy()) {
        this->SetVisible(false);
      }
    }
    return true;
  });
  AddChild(btn_ok);
}

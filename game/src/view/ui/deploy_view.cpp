#include "deploy_view.h"
#include "core/hero.h"
#include "ui/button_view.h"
#include "ui/image_view.h"
#include "ui/text_view.h"
#include "misc.h"

HeroModelView::HeroModelView(const Rect& frame, shared_ptr<Hero> hero) : CompositeView(frame), hero_(hero) {
  SetPadding(4);
  Rect img_src_rect(0, 0, 48, 48);
  Rect iv_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), img_src_rect.GetSize(), LayoutHelper::kAlignCenter);
  iv_frame.SetY(iv_frame.GetY() - 8);
  ImageView* iv_hero = new ImageView(iv_frame, Misc::GetModelPath(hero->GetBitmapPath(), kSpriteStand));
  iv_hero->SetSourceRect(img_src_rect);
  AddChild(iv_hero);

  Rect tv_frame = GetActualFrame();
  TextView* tv_hero = new TextView(&tv_frame, hero_->GetId(), COLOR("white"), 14, LayoutHelper::kAlignMidBot);
  AddChild(tv_hero);
}

HeroModelListView::HeroModelListView(const Rect& frame, const vector<shared_ptr<Hero>>& hero_list) : CompositeView(frame) {
  SetBgColor(COLOR("navy"));
  static const Vec2D kHeroModelSize = {96, 80};
  Rect hero_model_frame({0, 0}, kHeroModelSize);
  for (auto hero : hero_list) {
    HeroModelView* hero_model_view = new HeroModelView(hero_model_frame, hero);
    AddChild(hero_model_view);
    hero_model_frame.SetX(hero_model_frame.GetX() + kHeroModelSize.x);
    if (hero_model_frame.GetRight() > GetActualFrameSize().x) {
      hero_model_frame.SetX(0);
      hero_model_frame.SetY(hero_model_frame.GetY() + kHeroModelSize.y);
    }
  }
}

DeployView::DeployView(const Rect& frame, const vector<shared_ptr<Hero>>& hero_list)
    : CompositeView(frame), hero_list_(hero_list) {
  SetPadding(8);
  SetBgColor(COLOR("darkgray"));
  Rect hero_model_list_frame = GetActualFrame();
  hero_model_list_frame.SetW(5 * 96);
  HeroModelListView* hero_model_list_view = new HeroModelListView(hero_model_list_frame, hero_list_);
  AddChild(hero_model_list_view);

  Rect btn_ok_frame = LayoutHelper::CalcPosition(GetActualFrameSize(), {100, 50}, LayoutHelper::kAlignRgtBot);
  ButtonView* btn_ok = new ButtonView(&btn_ok_frame, "To Battle");
  AddChild(btn_ok);
}

#include "unit_over_view.h"

#include "core/unit.h"
#include "gui/uifw/gauge_view.h"
#include "gui/uifw/image_view.h"
#include "gui/uifw/text_view.h"
#include "resource_path.h"

namespace mengde {
namespace gui {
namespace app {

UnitOverView::UnitOverView(const Rect& frame) : CompositeView(frame), unit_(NULL) {
  bg_color(COLOR("darkgray"));

  Rect portrait_frame = {0, 0, 64, 80};
  iv_portrait_ = new ImageView(portrait_frame);
  Rect top_frame = {64 + LayoutHelper::kDefaultSpace, 0, 200, 16};
  tv_name_ = new TextView(top_frame, "name");
  top_frame.Move({0, 16});
  tv_lv_ = new TextView(top_frame, "lv");
  AddChild(iv_portrait_);
  AddChild(tv_name_);
  AddChild(tv_lv_);

  const Vec2D frame_space = {0, 24};
  const int kLabelWidth = 45;
  const int kLabelHeight = 16;

  Rect tv_frame = {0, 92, kLabelWidth, kLabelHeight};

  static const char* kStatNames[kNumGVs] = {"Exp", "HP", "MP", "Atk", "Def", "Dex", "Itl", "Mor"};
  for (int i = 0; i < kNumGVs; i++) {
    TextView* tv = new TextView(tv_frame, kStatNames[i]);
    tv_stats_[i] = tv;
    AddChild(tv);
    tv_frame.Move(frame_space);
  }

  Rect gv_frame = {kLabelWidth, 92, 220 - 16 - kLabelWidth, kLabelHeight};

  static const Color colors[kNumGVs] = {COLOR("gauge_exp"),   COLOR("gauge_hp"),    COLOR("gauge_mp"),
                                        COLOR("gauge_stats"), COLOR("gauge_stats"), COLOR("gauge_stats"),
                                        COLOR("gauge_stats"), COLOR("gauge_stats")};

  for (int i = 0; i < kNumGVs; i++) {
    GaugeView* gv = new GaugeView(gv_frame, 0, 1, 0, colors[i], COLOR(128, 128, 128), COLOR(255, 232, 142));
    if (i < kNumExp + kNumHpMp) {
      gv->SetHelpTextType(GaugeView::kHelpTextCurMax);
      gv->SetHelpTextAlign(LayoutHelper::kAlignCenter);
    } else {
      gv->SetHelpTextType(GaugeView::kHelpTextCurOnly);
      gv->SetHelpTextAlign(LayoutHelper::kAlignRgtMid);
    }
    gv_stats_[i] = gv;
    AddChild(gv);
    gv_frame.Move(frame_space);
  }
}

void UnitOverView::SetUnit(const core::IUnitBase* unit) {
  unit_ = unit;
  OnUnitUpdate();
}

void UnitOverView::OnUnitUpdate() {
  ASSERT(unit_ != NULL);

  std::string str_name = unit_->id();
  std::string str_lv = "Lv " + std::to_string(unit_->GetLevel());
  tv_name_->SetText(str_name);
  tv_lv_->SetText(str_lv);

  const core::Attribute& ori_attr = unit_->GetOriginalAttr();
  const core::Attribute& cur_attr = unit_->GetCurrentAttr();
  const core::HpMp& ori_hpmp = unit_->GetOriginalHpMp();
  const core::HpMp& cur_hpmp = unit_->GetCurrentHpMp();

  gv_stats_[0]->SetCurVal(unit_->GetExp());
  gv_stats_[0]->SetMaxVal(core::Level::kExpLimit);

  gv_stats_[1]->SetCurVal(cur_hpmp.hp);
  gv_stats_[1]->SetMaxVal(ori_hpmp.hp);
  gv_stats_[2]->SetCurVal(cur_hpmp.mp);
  gv_stats_[2]->SetMaxVal(ori_hpmp.mp);

  for (int i = 0; i < kNumStats; i++) {
    int j = i + 1 + kNumHpMp;
    gv_stats_[j]->SetCurVal(cur_attr.GetValueByIndex(i));
    gv_stats_[j]->SetMaxVal(400);
    gv_stats_[j]->SetExtVal(cur_attr.GetValueByIndex(i) - ori_attr.GetValueByIndex(i));
  }
  Path portrait_path = rcpath::PortraitPath(unit_->id());
  iv_portrait_->SetPath(portrait_path.ToString());
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

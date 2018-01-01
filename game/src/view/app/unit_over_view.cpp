#include "unit_over_view.h"
#include "core/unit.h"
#include "view/uifw/gauge_view.h"
#include "view/uifw/image_view.h"
#include "view/uifw/text_view.h"

UnitOverView::UnitOverView(const Rect* frame)
    : CompositeView(frame), unit_(NULL) {
  SetBgColor(COLOR("darkgray"));

  Rect portrait_frame = {0, 0, 64, 80};
  iv_portrait_ = new ImageView(&portrait_frame);
  Rect top_frame = {64 + LayoutHelper::kDefaultSpace, 0, 200, 16};
  tv_name_ = new TextView(&top_frame, "name");
  top_frame.Move({0, 16});
  tv_lv_ = new TextView(&top_frame, "lv");
  AddChild(iv_portrait_);
  AddChild(tv_name_);
  AddChild(tv_lv_);

  const Vec2D frame_space = {0, 24};
  const int kLabelWidth  = 45;
  const int kLabelHeight = 16;

  Rect tv_frame = {0, 92, kLabelWidth, kLabelHeight};

  static const char* kStatNames[kNumGVs] = {"HP", "MP", "Exp", "Atk", "Def", "Dex", "Itl", "Mor"};
  for (int i = 0; i < kNumGVs; i++) {
    TextView* tv = new TextView(&tv_frame, kStatNames[i]);
    tv_stats_[i] = tv;
    AddChild(tv);
    tv_frame.Move(frame_space);
  }

  Rect gv_frame = {kLabelWidth, 92, 220-16-kLabelWidth, kLabelHeight};
  static const Color colors[kNumXtats] = {COLOR("gauge_hp"),
                                          COLOR("gauge_mp"),
                                          COLOR("gauge_exp")};

  for (int i = 0; i < kNumXtats; i++) {
    GaugeView* gv = new GaugeView(&gv_frame,
                                  1,
                                  1,
                                  colors[i],
                                  COLOR(128, 128, 128));

    gv->SetHelpTextType(GaugeView::kHelpTextCurMax);
    gv_stats_[i] = gv;
    AddChild(gv);
    gv_frame.Move(frame_space);
  }
  for (int i = 0; i < kNumStats; i++) {
    int j = i + kNumXtats;
    int max_value = 300; // FIXME max value of stat
    GaugeView* gv = new GaugeView(&gv_frame,
                                  1,
                                  max_value,
                                  1,
                                  COLOR("gauge_stats"),
                                  COLOR(128, 128, 128),
                                  COLOR(255, 232, 142));
    gv->SetHelpTextType(GaugeView::kHelpTextCurOnly);
    gv->SetHelpTextAlign(LayoutHelper::kAlignRgtMid);
    gv_stats_[j] = gv;
    AddChild(gv);
    gv_frame.Move(frame_space);
  }
}

void UnitOverView::SetUnit(Unit* unit) {
  unit_ = unit;
  OnUnitUpdate();
}

void UnitOverView::OnUnitUpdate() {
  ASSERT(unit_ != NULL);

  std::string str_name = unit_->GetId();
  std::string str_lv   = "Lv " + std::to_string(unit_->GetLevel());
  tv_name_->SetText(str_name);
  tv_lv_->SetText(str_lv);

  const Stat& ori_stat = unit_->GetOriginalStat();
  const Stat& cur_stat = unit_->GetCurrentStat();
  const Xtat& ori_xtat = unit_->GetOriginalXtat();
  const Xtat& cur_xtat = unit_->GetCurrentXtat();
  for (int i = 0; i < kNumXtats; i++) {
    int max_val = (i == 2) ? unit_->GetMaxExp() : ori_xtat.GetValueByIndex(i);
    gv_stats_[i]->SetCurVal(cur_xtat.GetValueByIndex(i));
    gv_stats_[i]->SetMaxVal(max_val);
  }
  for (int i = 0; i < kNumStats; i++) {
    int j = i + kNumXtats;
    gv_stats_[j]->SetCurVal(cur_stat.GetValueByIndex(i));
    gv_stats_[j]->SetExtVal(cur_stat.GetValueByIndex(i) - ori_stat.GetValueByIndex(i));
  }
  string portrait_path = "portrait/" + unit_->GetId() + ".bmp";
  iv_portrait_->SetPath(portrait_path);
}

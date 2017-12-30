#include "unit_view.h"
#include "../color.h"
#include "../drawer.h"
#include "gauge_view.h"
#include "button_view.h"
#include "text_view.h"
#include "image_view.h"
#include "rect_view.h"
#include "core/hero.h"
#include "core/unit.h"

UnitView::UnitView(const Rect* frame)
    : TabView(frame), unit_(NULL) {
  Vec2D size = frame->GetSize();
  size -= {0, -28};
  Rect subframe({0, 28}, size);
  unit_over_view_ = new UnitOverView(&subframe);
  unit_equipment_view_ = new UnitEquipmentView(&subframe);
  AddTab("Unit", unit_over_view_);
  AddTab("Equip", unit_equipment_view_);
//  SetViewIndex(1);
}

void UnitView::SetUnit(Unit* unit) {
  if (unit_ != unit) {
    unit_ = unit;
    unit_over_view_->SetUnit(unit);
    unit_equipment_view_->SetUnit(unit);
  }
}

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

UnitEquipmentView::UnitEquipmentView(const Rect* frame)
    : CompositeView(frame), unit_(NULL) {
  const int kMargin = 80 + 8;
  Rect tv_frame = {0, 0, 200, 16};
  tv_weapon_label_ = new TextView(&tv_frame, "Weapon");
  tv_frame.Move({0, kMargin});
  tv_armor_label_  = new TextView(&tv_frame, "Armor");
  tv_frame.Move({0, kMargin});
  tv_aid_label_    = new TextView(&tv_frame, "Aid");
  AddChild(tv_weapon_label_);
  AddChild(tv_armor_label_);
  AddChild(tv_aid_label_);

  Equipment* weapon = NULL;
  Equipment* armor = NULL;
  Equipment* aid = NULL;
  if (unit_ != NULL) {
    weapon = unit_->GetWeapon();
    armor = unit_->GetArmor();
    aid = unit_->GetAid();
  }
  Rect equipment_view_frame = {0, 24, 204, 60};
  eqv_weapon_ = new EquipmentView(&equipment_view_frame, weapon);
  equipment_view_frame.Move({0, equipment_view_frame.GetH() + 24});
  eqv_armor_ = new EquipmentView(&equipment_view_frame, armor);
  equipment_view_frame.Move({0, equipment_view_frame.GetH() + 24});
  eqv_aid_ = new EquipmentView(&equipment_view_frame, aid);
  AddChild(eqv_weapon_);
  AddChild(eqv_armor_);
  AddChild(eqv_aid_);
}

void UnitEquipmentView::SetUnit(Unit* unit) {
  unit_ = unit;
  OnUnitUpdate();
}

void UnitEquipmentView::OnUnitUpdate() {
  ASSERT(unit_ != NULL);
  eqv_weapon_->SetEquipment(unit_->GetWeapon());
  eqv_armor_->SetEquipment(unit_->GetArmor());
  eqv_aid_->SetEquipment(unit_->GetAid());
}

EquipmentView::EquipmentView(const Rect* frame, Equipment* equipment) : CompositeView(frame) {
  Rect iv_frame = LayoutHelper::CalcPosition(GetActualFrame().GetSize(),
                                             {32, 32},
                                             LayoutHelper::kAlignLftMid);
  iv_image_ = new ImageView(&iv_frame, "equipments/50-1.bmp"); // FIXME path hardcoded
  Rect tv_name_frame = {32 + 8, 0, 164, 16};
  tv_name_  = new TextView(&tv_name_frame, "");
  Rect tv_desc_frame = {32 + 8, 16, 164, 52};
  tv_desc_  = new TextView(&tv_desc_frame, "");
  AddChild(iv_image_);
  AddChild(tv_name_);
  AddChild(tv_desc_);

  SetEquipment(equipment);
}

void EquipmentView::SetEquipment(Equipment* equipment) {
  string image_path = "equipments/60-1.bmp";
  string name = "No Equipment";
  string desc = "No Desc";
  if (equipment != NULL) {
    image_path = "equipments/" + equipment->GetId() + ".bmp";
    name = equipment->GetId();
    desc = equipment->GetId() + "_desc";
  }

  iv_image_->SetPath(image_path);
  tv_name_->SetText(name);
  tv_desc_->SetText(desc);
}


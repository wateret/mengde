#include "unit_info_view.h"
#include "rect.h"
#include "app.h"
#include "unit.h"
#include "cell.h"
#include "gauge_view.h"
#include "drawer.h"

UnitInfoView::UnitInfoView(const Rect* frame, Unit* unit)
    : View(frame),
      unit_(unit),
      gv_hp_(NULL),
      gv_mp_(NULL),
      str_lftbot_(),
      str_rgtbot_() {
  SetBgColor({64, 64, 64, 255});
  SetPadding(8);

  Rect gv_frame = {0, 22, 184, 16};
  gv_hp_ = new GaugeView(&gv_frame,
                         0,
                         1,
                         0,
                         COLORC_GAUGE_HP,
                         COLORC_GAUGE_BG,
                         COLORC_GAUGE_HPRED);
  gv_frame.Move(0, 22);
  gv_mp_ = new GaugeView(&gv_frame,
                         0,
                         1,
                         COLORC_GAUGE_MP,
                         COLORC_GAUGE_BG);
  gv_hp_->SetHelpTextType(GaugeView::kHelpTextCurMax);
  gv_mp_->SetHelpTextType(GaugeView::kHelpTextCurMax);
}

UnitInfoView::~UnitInfoView() {
  ASSERT(gv_hp_ != NULL);
  ASSERT(gv_mp_ != NULL);
  delete gv_hp_;
  delete gv_mp_;
}

void UnitInfoView::SetUnitTerrainInfo(Cell* cell) {
  ASSERT(cell->IsUnitPlaced());

  Unit* unit = cell->GetUnit();
  if (unit_ == unit) return;
  SetUnit(unit);

  string name = cell->GetTerrainName();
  int effect = cell->GetTerrainEffectThisCell();
  string terrain_effect = name + " " + std::to_string(effect) + "%";
  str_rgtbot_ = terrain_effect;
}

void UnitInfoView::SetUnitAttackInfo(Unit* unit, int accuracy, int expected_damage) {
  SetUnit(unit);
  gv_hp_->SetExtVal(expected_damage);
  str_rgtbot_ = "Accuracy : " +
                std::to_string(accuracy) +
                "%";
}

void UnitInfoView::SetUnit(Unit* unit) {
  unit_ = unit;
  const Xtat* cur_xtat = unit_->GetCurrentXtat();
  const Xtat* ori_xtat = unit_->GetOriginalXtat();
  gv_hp_->SetCurVal(cur_xtat->hp);
  gv_hp_->SetMaxVal(ori_xtat->hp);
  gv_mp_->SetCurVal(cur_xtat->mp);
  gv_mp_->SetMaxVal(ori_xtat->mp);
}

void UnitInfoView::RenderView(Drawer* drawer) {
  gv_hp_->Render(drawer);
  gv_mp_->Render(drawer);

  string str_name = unit_->GetId();
  string str_lv = "Lv " + std::to_string(unit_->GetLevel());
  Rect conv(Vec2D(0, 0), GetActualFrameSize());
  drawer->DrawTextAbs(str_name, 14, COLOR_WHITE, &conv, LayoutHelper::kAlignLftTop);
  drawer->DrawTextAbs(str_lv, 14, COLOR_WHITE, &conv, LayoutHelper::kAlignRgtTop);
  drawer->DrawTextAbs(str_lftbot_, 14, COLOR_WHITE, &conv, LayoutHelper::kAlignLftBot);
  drawer->DrawTextAbs(str_rgtbot_, 14, COLOR_WHITE, &conv, LayoutHelper::kAlignRgtBot);
}

bool UnitInfoView::OnMouseMotionEvent(const MouseMotionEvent) {
  return true;
}

#include "unit_info_view.h"
#include "rect.h"
#include "app.h"
#include "unit.h"
#include "cell.h"
#include "gauge_view.h"
#include "text_view.h"
#include "drawer.h"

UnitInfoView::UnitInfoView(const Rect* frame, Unit* unit)
    : CompositeView(frame),
      unit_(unit),
      gv_hp_(nullptr),
      gv_mp_(nullptr),
      tv_name_(nullptr),
      tv_lv_(nullptr),
      tv_lftbot_(nullptr),
      tv_rgtbot_(nullptr) {
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

  Rect tv_rect = GetActualFrame();
  tv_name_   = new TextView(&tv_rect, "", COLOR_WHITE, 14, LayoutHelper::kAlignLftTop);
  tv_lv_     = new TextView(&tv_rect, "", COLOR_WHITE, 14, LayoutHelper::kAlignRgtTop);
  tv_lftbot_ = new TextView(&tv_rect, "", COLOR_WHITE, 14, LayoutHelper::kAlignLftBot);
  tv_rgtbot_ = new TextView(&tv_rect, "", COLOR_WHITE, 14, LayoutHelper::kAlignRgtBot);

  AddChild(gv_hp_);
  AddChild(gv_mp_);
  AddChild(tv_name_);
  AddChild(tv_lv_);
  AddChild(tv_lftbot_);
  AddChild(tv_rgtbot_);
}

UnitInfoView::~UnitInfoView() {
}

void UnitInfoView::SetUnitTerrainInfo(Cell* cell) {
  ASSERT(cell->IsUnitPlaced());

  Unit* unit = cell->GetUnit();
  if (unit_ == unit) return;
  SetUnit(unit);

  string name = cell->GetTerrainName();
  int effect = cell->GetTerrainEffectThisCell();
  string terrain_effect = name + " " + std::to_string(effect) + "%";
  tv_rgtbot_->SetText(terrain_effect);
}

void UnitInfoView::SetUnitAttackInfo(Unit* unit, int accuracy, int expected_damage) {
  SetUnit(unit);
  gv_hp_->SetExtVal(expected_damage);
  tv_rgtbot_->SetText("Accuracy : " + std::to_string(accuracy) + "%");
}

void UnitInfoView::SetUnit(Unit* unit) {
  unit_ = unit;
  const Xtat* cur_xtat = unit_->GetCurrentXtat();
  const Xtat* ori_xtat = unit_->GetOriginalXtat();
  gv_hp_->SetCurVal(cur_xtat->hp);
  gv_hp_->SetMaxVal(ori_xtat->hp);
  gv_mp_->SetCurVal(cur_xtat->mp);
  gv_mp_->SetMaxVal(ori_xtat->mp);
  tv_name_->SetText(unit_->GetId());
  tv_lv_->SetText("Lv " + std::to_string(unit_->GetLevel()));
}

void UnitInfoView::SetCoordsByUnitCoords(Vec2D unit_cell, Vec2D camera_coords) {
  Vec2D c = ((unit_cell + 1) * 48) - camera_coords;
  SetCoords(c);
}

bool UnitInfoView::OnMouseMotionEvent(const MouseMotionEvent) {
  return true;
}

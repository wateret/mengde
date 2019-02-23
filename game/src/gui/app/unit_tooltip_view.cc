#include "unit_tooltip_view.h"

#include "core/cell.h"
#include "core/unit.h"
#include "gui/foundation/rect.h"
#include "gui/uifw/drawer.h"
#include "gui/uifw/gauge_view.h"
#include "gui/uifw/text_view.h"
#include "layout_helper.h"

namespace mengde {
namespace gui {
namespace app {

UnitTooltipView::UnitTooltipView(const Rect& frame, const core::Unit* unit)
    : CompositeView(frame),
      unit_(unit),
      gv_hp_(nullptr),
      gv_mp_(nullptr),
      tv_name_(nullptr),
      tv_lv_(nullptr),
      tv_lftbot_(nullptr),
      tv_rgtbot_(nullptr) {
  const int kAlpha = 160;
  bg_color({64, 64, 64, kAlpha});
  padding(8);

  Rect gv_frame = {0, 22, 184, 16};
  gv_hp_ = new GaugeView(gv_frame, 0, 1, 0, COLOR("gauge_hp", kAlpha), COLOR("gauge_bg", kAlpha),
                         COLOR("gauge_hp_damage", kAlpha));
  gv_frame.Move(0, 22);
  gv_mp_ = new GaugeView(gv_frame, 0, 1, COLOR("gauge_mp", kAlpha), COLOR("gauge_bg", kAlpha));
  gv_hp_->SetHelpTextType(GaugeView::kHelpTextCurMax);
  gv_mp_->SetHelpTextType(GaugeView::kHelpTextCurMax);

  Rect tv_rect = GetActualFrame();
  tv_name_ = new TextView(tv_rect, "", COLOR("white"), 14, LayoutHelper::kAlignLftTop);
  tv_lv_ = new TextView(tv_rect, "", COLOR("white"), 14, LayoutHelper::kAlignRgtTop);
  tv_lftbot_ = new TextView(tv_rect, "", COLOR("white"), 14, LayoutHelper::kAlignLftBot);
  tv_rgtbot_ = new TextView(tv_rect, "", COLOR("white"), 14, LayoutHelper::kAlignRgtBot);

  AddChild(gv_hp_);
  AddChild(gv_mp_);
  AddChild(tv_name_);
  AddChild(tv_lv_);
  AddChild(tv_lftbot_);
  AddChild(tv_rgtbot_);
}

UnitTooltipView::~UnitTooltipView() {}

void UnitTooltipView::SetUnitTerrainInfo(const core::Cell* cell, const core::Unit* unit) {
  ASSERT(cell->IsUnitPlaced());

  if (unit_ == unit) return;
  SetUnit(unit);

  string id = cell->GetTerrainId();

  // TODO cell->GetTerrainEffectThisCell() is unavailable. Need another interface for this.
  // int effect = cell->GetTerrainEffectThisCell();
  int effect = 0;  // FIXME delete this

  string terrain_effect = id + " " + std::to_string(effect) + "%";

  tv_rgtbot_->SetText(terrain_effect);
}

void UnitTooltipView::SetUnitActionInfo(const core::Unit* unit, int accuracy, int hp_diff) {
  SetUnit(unit);

  const core::HpMp& cur_xtat = unit_->GetCurrentHpMp();

  if (hp_diff < 0) {
    gv_hp_->SetCurVal(cur_xtat.hp);
    gv_hp_->SetExtVal(-hp_diff);
    SetMode(true, false /* MP - Dummy value */);
  } else {
    gv_hp_->SetCurVal(cur_xtat.hp + hp_diff);
    gv_hp_->SetExtVal(hp_diff);
    SetMode(false, false /* MP - Dummy value */);
  }

  tv_rgtbot_->SetText("Accuracy : " + std::to_string(accuracy) + "%");
}

void UnitTooltipView::SetUnit(const core::Unit* unit) {
  unit_ = unit;
  const core::HpMp& cur_xtat = unit_->GetCurrentHpMp();
  const core::HpMp& ori_xtat = unit_->GetOriginalHpMp();
  gv_hp_->SetCurVal(cur_xtat.hp);
  gv_hp_->SetMaxVal(ori_xtat.hp);
  gv_hp_->SetExtVal(0);
  gv_mp_->SetCurVal(cur_xtat.mp);
  gv_mp_->SetMaxVal(ori_xtat.mp);
  gv_mp_->SetExtVal(0);
  tv_name_->SetText(unit_->id());
  tv_lv_->SetText("Lv " + std::to_string(unit_->GetLevel()));
}

void UnitTooltipView::SetContents(const std::string& name, int lv, const core::HpMp& hpmp_cur,
                                  const core::HpMp& hpmp_max, const core::HpMp& hpmp_ext, bool hp_mode, bool mp_mode) {
  unit_ = nullptr;  // Reset cache
  gv_hp_->SetCurVal(hpmp_cur.hp);
  gv_hp_->SetMaxVal(hpmp_max.hp);
  gv_hp_->SetExtVal(hpmp_ext.hp);
  gv_mp_->SetCurVal(hpmp_cur.mp);
  gv_mp_->SetMaxVal(hpmp_max.mp);
  gv_mp_->SetExtVal(hpmp_ext.mp);
  tv_name_->SetText(name);
  tv_lv_->SetText("Lv " + std::to_string(lv));
  tv_lftbot_->SetText("");
  tv_rgtbot_->SetText("");
  SetMode(hp_mode, mp_mode);
}

void UnitTooltipView::SetCoordsByUnitCoords(Vec2D unit, Vec2D camera, Vec2D game_frame) {
  SetCoords(layout::CalcPositionNearUnit(GetFrameSize(), game_frame, camera, unit));
}

bool UnitTooltipView::OnMouseMotionEvent(const foundation::MouseMotionEvent&) { return false; }

void UnitTooltipView::SetMode(bool hp_mode, bool mp_mode) {
  auto hp_color = hp_mode ? COLOR("gauge_hp_damage") : COLOR("gauge_hp_heal");
  auto mp_color = mp_mode ? COLOR("gauge_hp_damage") : COLOR("gauge_hp_heal");
  gv_hp_->SetExtColor(hp_color);
  gv_mp_->SetExtColor(mp_color);
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

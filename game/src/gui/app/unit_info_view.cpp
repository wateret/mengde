#include "unit_info_view.h"

#include "core/unit.h"
#include "core/cell.h"
#include "gui/uifw/gauge_view.h"
#include "gui/uifw/text_view.h"
#include "gui/uifw/drawer.h"
#include "gui/foundation/rect.h"

namespace mengde {
namespace gui {
namespace app {

UnitInfoView::UnitInfoView(const Rect* frame, core::Unit* unit)
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
  gv_hp_ = new GaugeView(&gv_frame,
                         0,
                         1,
                         0,
                         COLOR("gauge_hp", kAlpha),
                         COLOR("gauge_bg", kAlpha),
                         COLOR("gauge_hp_damage", kAlpha));
  gv_frame.Move(0, 22);
  gv_mp_ = new GaugeView(&gv_frame,
                         0,
                         1,
                         COLOR("gauge_mp", kAlpha),
                         COLOR("gauge_bg", kAlpha));
  gv_hp_->SetHelpTextType(GaugeView::kHelpTextCurMax);
  gv_mp_->SetHelpTextType(GaugeView::kHelpTextCurMax);

  Rect tv_rect = GetActualFrame();
  tv_name_   = new TextView(&tv_rect, "", COLOR("white"), 14, LayoutHelper::kAlignLftTop);
  tv_lv_     = new TextView(&tv_rect, "", COLOR("white"), 14, LayoutHelper::kAlignRgtTop);
  tv_lftbot_ = new TextView(&tv_rect, "", COLOR("white"), 14, LayoutHelper::kAlignLftBot);
  tv_rgtbot_ = new TextView(&tv_rect, "", COLOR("white"), 14, LayoutHelper::kAlignRgtBot);

  AddChild(gv_hp_);
  AddChild(gv_mp_);
  AddChild(tv_name_);
  AddChild(tv_lv_);
  AddChild(tv_lftbot_);
  AddChild(tv_rgtbot_);
}

UnitInfoView::~UnitInfoView() {
}

void UnitInfoView::SetUnitTerrainInfo(core::Cell* cell) {
  ASSERT(cell->IsUnitPlaced());

  core::Unit* unit = cell->GetUnit();
  if (unit_ == unit) return;
  SetUnit(unit);

  string name = cell->GetTerrainName();
  int effect = cell->GetTerrainEffectThisCell();
  string terrain_effect = name + " " + std::to_string(effect) + "%";
  tv_rgtbot_->SetText(terrain_effect);
}

void UnitInfoView::SetUnitAttackInfo(core::Unit* unit, int accuracy, int expected_damage) {
  SetUnit(unit);
  gv_hp_->SetExtVal(expected_damage);
  tv_rgtbot_->SetText("Accuracy : " + std::to_string(accuracy) + "%");
}

void UnitInfoView::SetUnit(core::Unit* unit) {
  unit_ = unit;
  const core::HpMp& cur_xtat = unit_->GetCurrentHpMp();
  const core::HpMp& ori_xtat = unit_->GetOriginalHpMp();
  gv_hp_->SetCurVal(cur_xtat.hp);
  gv_hp_->SetMaxVal(ori_xtat.hp);
  gv_hp_->SetExtVal(0);
  gv_mp_->SetCurVal(cur_xtat.mp);
  gv_mp_->SetMaxVal(ori_xtat.mp);
  gv_mp_->SetExtVal(0);
  tv_name_->SetText(unit_->GetId());
  tv_lv_->SetText("Lv " + std::to_string(unit_->GetLevel()));
}

void UnitInfoView::SetContents(const std::string& name,
                               int lv,
                               const core::HpMp& hpmp_cur,
                               const core::HpMp& hpmp_max,
                               const core::HpMp& hpmp_ext) {
  unit_ = nullptr; // Reset cache
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
}

bool UnitInfoView::OnMouseMotionEvent(const foundation::MouseMotionEvent) {
  return false;
}

} // namespace app
} // namespace gui
} // namespace mengde

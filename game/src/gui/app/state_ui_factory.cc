#include "state_ui_factory.h"

#include "core/unit.h"

namespace mengde {
namespace gui {
namespace app {

using namespace mengde::core;

void StateUIGenerator::Visit(const CmdQueue&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdPlayAI&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdGameVictory&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdAction&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdMove& cmd) {
  if (gi_->GetUnit(cmd.GetUnit())->position() == cmd.GetDest()) {
    generated_ = nullptr;
  } else {
    generated_ = new StateUIMoving(WrapBase(), cmd.GetUnit(), cmd.GetDest());
  }
}

void StateUIGenerator::Visit(const CmdBasicAttack&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdMagic&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdHit& cmd) {
  if (cmd.IsBasicAttack()) {
    generated_ = new StateUIAttack(WrapBase(), cmd.GetUnitAtk(), cmd.GetUnitDef(), true,
                                   cmd.GetHitType() == CmdHit::HitType::kCritical, cmd.GetDamage());
  } else {
    ASSERT(cmd.IsMagic());
    generated_ =
        new StateUIMagic(WrapBase(), cmd.GetUnitAtk(), cmd.GetUnitDef(), cmd.GetMagic(), true, cmd.GetDamage());
  }
}

void StateUIGenerator::Visit(const CmdMiss& cmd) {
  if (cmd.IsBasicAttack()) {
    generated_ = new StateUIAttack(WrapBase(), cmd.GetUnitAtk(), cmd.GetUnitDef(), false, false, 0);
  } else {
    ASSERT(cmd.IsMagic());
    generated_ = new StateUIMagic(WrapBase(), cmd.GetUnitAtk(), cmd.GetUnitDef(), cmd.GetMagic(), false, 0);
  }
}

void StateUIGenerator::Visit(const CmdKilled& cmd) { generated_ = new StateUIKilled(WrapBase(), cmd.GetUnit()); }

void StateUIGenerator::Visit(const CmdEndTurn&) { generated_ = new StateUINextTurn(WrapBase()); }

void StateUIGenerator::Visit(const CmdStay&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdSpeak& cmd) {
  generated_ = new StateUISpeak(WrapBase(), cmd.GetUnit() /* FIXME */, cmd.GetWords());
}

void StateUIGenerator::Visit(const CmdGameEnd& cmd) { generated_ = new StateUIEnd(WrapBase(), cmd.is_victory()); }

void StateUIGenerator::Visit(const CmdRestoreHp& cmd) {
  int amount = cmd.CalcAmount(gi_);
  if (amount == 0) {
    generated_ = nullptr;
  } else {
    generated_ = new StateUIUnitTooltipAnim(WrapBase(), cmd.GetUnit(), amount, 0);
  }
}

void StateUIGenerator::Visit(const CmdEndAction&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdGainExp&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdLevelUp&) { generated_ = nullptr; }

void StateUIGenerator::Visit(const CmdPromote& cmd) { generated_ = new StateUIPromote(WrapBase(), cmd.GetUnit()); }

}  // namespace app
}  // namespace gui
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

StateUI* StateUIFactory::GenerateNext(void) {
  ASSERT(gi_->HasNextCmd());
  const core::Cmd* cmd = gi_->GetNextCmd();
  StateUIGenerator generator{gi_, gv_};
  cmd->Accept(generator);
  return generator.TakeGenerated();
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

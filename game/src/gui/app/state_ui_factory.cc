#include "state_ui_factory.h"

namespace mengde {
namespace gui {
namespace app {

StateUI* StateUIFactory::GenerateNext(void) {
  ASSERT(gi_->HasNextCmd());
  const core::Cmd* cmd = gi_->GetNextCmd();
  StateUI* no_state_ui = nullptr;

#define DYNAMIC_CAST_CHECK(type)  \
  dynamic_cast<const type*>(cmd); \
  ASSERT(c != nullptr);

  switch (cmd->op()) {
    case core::Cmd::Op::kCmdAction:
    case core::Cmd::Op::kCmdPlayAI:
    case core::Cmd::Op::kCmdGameVictory:
      return no_state_ui;

    case core::Cmd::Op::kCmdHit: {
      const core::CmdHit* c = DYNAMIC_CAST_CHECK(core::CmdHit);
      if (c->IsBasicAttack()) {
        return new StateUIAttack(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), true,
                                 c->GetHitType() == core::CmdHit::HitType::kCritical, c->GetDamage());
      } else {
        ASSERT(c->IsMagic());
        return new StateUIMagic(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), c->GetMagic(), true, c->GetDamage());
      }
    }
    case core::Cmd::Op::kCmdMiss: {
      const core::CmdMiss* c = DYNAMIC_CAST_CHECK(core::CmdMiss);
      if (c->IsBasicAttack()) {
        return new StateUIAttack(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), false, false, 0);
      } else {
        ASSERT(c->IsMagic());
        return new StateUIMagic(WrapBase(), c->GetUnitAtk(), c->GetUnitDef(), c->GetMagic(), false, 0);
      }
    }
    case core::Cmd::Op::kCmdMove: {
      const core::CmdMove* c = DYNAMIC_CAST_CHECK(core::CmdMove);
      if (gi_->GetUnit(c->GetUnit())->GetPosition() == c->GetDest()) {
        return no_state_ui;
      } else {
        return new StateUIMoving(WrapBase(), c->GetUnit() /* FIXME */, c->GetDest());
      }
    }
    case core::Cmd::Op::kCmdKilled: {
      const core::CmdKilled* c = DYNAMIC_CAST_CHECK(core::CmdKilled);
      return new StateUIKilled(WrapBase(), c->GetUnit());
    }
    case core::Cmd::Op::kCmdEndTurn:
      return new StateUINextTurn(WrapBase());

    case core::Cmd::Op::kCmdSpeak: {
      const core::CmdSpeak* c = DYNAMIC_CAST_CHECK(core::CmdSpeak);
      return new StateUISpeak(WrapBase(), c->GetUnit() /* FIXME */, c->GetWords());
    }
    case core::Cmd::Op::kCmdGameEnd: {
      const core::CmdGameEnd* c = DYNAMIC_CAST_CHECK(core::CmdGameEnd);
      return new StateUIEnd(WrapBase(), c->is_victory());
    }
    case core::Cmd::Op::kCmdRestoreHp: {
      const core::CmdRestoreHp* c = DYNAMIC_CAST_CHECK(core::CmdRestoreHp);
      int amount = c->CalcAmount(gi_);
      if (amount == 0) return no_state_ui;
      return new StateUIUnitTooltipAnim(WrapBase(), c->GetUnit(), amount, 0);
    }

    default:
      //      UNREACHABLE("Unknown type of Cmd");
      return no_state_ui;
  }

  return no_state_ui;
}

}  // namespace app
}  // namespace gui
}  // namespace mengde

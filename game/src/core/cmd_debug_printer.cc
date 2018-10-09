#include "cmd_debug_printer.h"

#include "stage.h"

namespace mengde {
namespace core {

CmdDebugPrinter::CmdDebugPrinter(const Stage* stage) : stage_(stage) {}

string CmdDebugPrinter::CmdToString(const Cmd& cmd) { return string{kCmdOpToString[static_cast<uint32_t>(cmd.op())]}; }

string CmdDebugPrinter::CmdUnitToString(const CmdUnit& cmd) {
  auto unit = cmd.GetUnit();
  string unit_id = (unit ? stage_->LookupUnit(unit)->id() : "N/A");
  return CmdToString(cmd) + " {" + unit_id + "}";
}

string CmdDebugPrinter::CmdTwoUnitsToString(const CmdTwoUnits& cmd) {
  auto atk = cmd.GetUnitAtk();
  auto def = cmd.GetUnitDef();
  string atk_id = (atk ? stage_->LookupUnit(atk)->id() : "N/A");
  string def_id = (def ? stage_->LookupUnit(def)->id() : "N/A");
  return CmdToString(cmd) + " {" + atk_id + "}->{" + def_id + "}";
}

void CmdDebugPrinter::Visit(const CmdQueue& cmd) {
  dumped_ = CmdToString(cmd);
  dumped_ += " {\n";
  for (auto&& e : cmd) {
    CmdDebugPrinter printer{stage_};
    e->Accept(printer);
    auto child_str = "  " + printer.TakeDumped() + "\n";
    dumped_ += child_str;
  }
  dumped_ += "}";
}

void CmdDebugPrinter::Visit(const CmdPlayAI& cmd) { dumped_ = CmdToString(cmd); }

void CmdDebugPrinter::Visit(const CmdGameVictory& cmd) { dumped_ = CmdToString(cmd); }

void CmdDebugPrinter::Visit(const CmdAction& cmd) { dumped_ = CmdToString(cmd); }

void CmdDebugPrinter::Visit(const CmdMove& cmd) { dumped_ = CmdUnitToString(cmd); }

void CmdDebugPrinter::Visit(const CmdBasicAttack& cmd) { dumped_ = CmdTwoUnitsToString(cmd); }

void CmdDebugPrinter::Visit(const CmdMagic& cmd) { dumped_ = CmdTwoUnitsToString(cmd); }

void CmdDebugPrinter::Visit(const CmdHit& cmd) {
  dumped_ = CmdToString(cmd);
  dumped_ += " (damage:" + std::to_string(cmd.GetDamage()) + ")";
}

void CmdDebugPrinter::Visit(const CmdMiss& cmd) { dumped_ = CmdTwoUnitsToString(cmd); }

void CmdDebugPrinter::Visit(const CmdKilled& cmd) { dumped_ = CmdUnitToString(cmd); }

void CmdDebugPrinter::Visit(const CmdEndTurn& cmd) { dumped_ = CmdToString(cmd); }

void CmdDebugPrinter::Visit(const CmdStay& cmd) { dumped_ = CmdTwoUnitsToString(cmd); }

void CmdDebugPrinter::Visit(const CmdSpeak& cmd) { dumped_ = CmdUnitToString(cmd); }

void CmdDebugPrinter::Visit(const CmdGameEnd& cmd) { dumped_ = CmdToString(cmd); }

void CmdDebugPrinter::Visit(const CmdRestoreHp& cmd) { dumped_ = CmdUnitToString(cmd); }

void CmdDebugPrinter::Visit(const CmdEndAction& cmd) { dumped_ = CmdUnitToString(cmd); }

void CmdDebugPrinter::Visit(const CmdGainExp& cmd) { dumped_ = CmdUnitToString(cmd); }

void CmdDebugPrinter::Visit(const CmdLevelUp& cmd) {
  dumped_ = CmdUnitToString(cmd);

  const auto unit = stage_->LookupUnit(cmd.GetUnit());
  dumped_ += " (level-from:" + std::to_string(unit->GetLevel()) + ")";
}

void CmdDebugPrinter::Visit(const CmdPromote& cmd) { dumped_ = CmdUnitToString(cmd); }

}  // namespace core
}  // namespace mengde

#ifndef MENGDE_CORE_CMD_DEBUG_PRINTER_H_
#define MENGDE_CORE_CMD_DEBUG_PRINTER_H_

#include "cmd.h"
#include "cmd_visitor.h"

namespace mengde {
namespace core {

class CmdDebugPrinter : public CmdVisitor {
 public:
  CmdDebugPrinter(const Stage* stage);

 public:
#define MACRO_CMD_OP(name) virtual void Visit(const Cmd##name& cmd) override final;
#include "cmd_op.h.inc"

 public:
  string TakeDumped() { return dumped_; }

 private:
  string CmdToString(const Cmd& cmd);
  string CmdUnitToString(const CmdUnit& cmd);
  string CmdTwoUnitsToString(const CmdTwoUnits& cmd);

 private:
  const Stage* stage_;
  string dumped_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_CMD_DEBUG_PRINTER_H_

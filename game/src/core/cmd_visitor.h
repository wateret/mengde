#ifndef MENGDE_CORE_CMD_VISITOR_H_
#define MENGDE_CORE_CMD_VISITOR_H_

#include "cmd.h"

namespace mengde {
namespace core {

class CmdVisitor {
 public:
  virtual ~CmdVisitor() = default;

 public:
#define MACRO_CMD_OP(name) virtual void Visit(const Cmd##name& cmd) = 0;
#include "cmd_op.h.inc"
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_CMD_VISITOR_H_

#ifndef MENGDE_CORE_COMMAND_H_
#define MENGDE_CORE_COMMAND_H_

#include <boost/optional.hpp>

#include "id.h"
#include "util/common.h"

namespace mengde {
namespace core {

class CmdVisitor;
class Stage;
class UserInterface;
class Map;

extern const char* kCmdOpToString[];

class Cmd {
 public:
  enum class Op {
    kCmdInvalid,
#define MACRO_CMD_OP(name) kCmd##name,
#include "cmd_op.h.inc"
  };

 public:
  virtual ~Cmd() = default;
  virtual unique_ptr<Cmd> Do(Stage*) = 0;
  // virtual Cmd* Undo(Stage*) = 0;
  virtual Cmd::Op op() const = 0;

 public:
  virtual void Accept(CmdVisitor& visitor) const = 0;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_COMMAND_H_

#ifndef COMMANDER_H_
#define COMMANDER_H_

#include "util/common.h"

namespace mengde {
namespace core {

class Cmd;
class CmdQueue;
class Game;

class Commander {
 public:
  Commander();
  bool HasNext() const;
  const Cmd* GetNextCmdConst() const;
  void DoNext(Game*);
//  void UnDo(Game*);
  void Push(unique_ptr<Cmd>);
  void DebugPrint() const;

 private:
  unique_ptr<CmdQueue> cmdq_current_;
  unique_ptr<CmdQueue> cmdq_history_;
};

} // namespace core
} // namespace mengde

#endif // COMMANDER_H_

#ifndef COMMANDER_H_
#define COMMANDER_H_

#include "util/common.h"

class Cmd;
class CmdQueue;
class Game;

class Commander {
 public:
  Commander();
  bool HasPendingCmd() const;
  const Cmd* GetNextCmdConst() const;
  void DoNextCmd(Game*);
//  void UnDo(Game*);
  void PushCmd(unique_ptr<Cmd>);
  void DebugPrint() const;

 private:
  unique_ptr<CmdQueue> cmdq_current_;
  unique_ptr<CmdQueue> cmdq_history_;
};

#endif // COMMANDER_H_

#ifndef MENGDE_CORE_COMMANDER_H_
#define MENGDE_CORE_COMMANDER_H_

#include "util/common.h"

namespace mengde {
namespace core {

class Cmd;
class CmdQueue;
class Stage;

class Commander {
 public:
  Commander();
  bool HasNext() const;
  const Cmd* GetNextCmdConst() const;
  void DoNext(Stage*);
  //  void UnDo(Stage*);
  void Push(unique_ptr<Cmd>);

 public:
  const CmdQueue& queue() const { return *cmdq_current_.get(); }

 private:
  unique_ptr<CmdQueue> cmdq_current_;
  unique_ptr<CmdQueue> cmdq_history_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_COMMANDER_H_

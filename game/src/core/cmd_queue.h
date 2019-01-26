#ifndef MENGDE_CORE_CMD_QUEUE_H_
#define MENGDE_CORE_CMD_QUEUE_H_

#include "cmd.h"

namespace mengde {
namespace core {

class CmdQueue : public Cmd {
 public:
  CmdQueue() = default;
  ~CmdQueue() = default;
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdQueue; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 public:
  void Prepend(unique_ptr<Cmd>);
  void Append(unique_ptr<Cmd>);
  bool IsEmpty() const;
  const Cmd* GetNextCmdConst() const;

  CmdQueue& operator+=(unique_ptr<Cmd>);

 public:
  deque<unique_ptr<Cmd>>::const_iterator begin() const { return q_.begin(); }
  deque<unique_ptr<Cmd>>::const_iterator end() const { return q_.end(); }

 private:
  void Insert(unique_ptr<Cmd>, bool prepend);

 private:
  deque<unique_ptr<Cmd>> q_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_CMD_QUEUE_H_

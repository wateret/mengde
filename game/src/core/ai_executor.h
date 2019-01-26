#ifndef MENGDE_CORE_AI_EXECUTOR_H_
#define MENGDE_CORE_AI_EXECUTOR_H_

#include "cmd.h"

namespace mengde {
namespace core {

class UserInterface;

class AIExecutor {
 public:
  AIExecutor(UserInterface* ui);

  unique_ptr<Cmd> Step();

 private:
  UserInterface* ui_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_AI_EXECUTOR_H_

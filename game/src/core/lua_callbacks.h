#ifndef MENGDE_CORE_LUA_CALLBACKS_H_
#define MENGDE_CORE_LUA_CALLBACKS_H_

#include "lua/ref.h"

namespace mengde {
namespace core {

class LuaCallbacks {
 public:
  LuaCallbacks() = default;
  void end_condition(const lua::Ref& ref) { end_condition_ = ref; }
  const lua::Ref& end_condition() const { return end_condition_; }

 private:
  lua::Ref end_condition_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_LUA_CALLBACKS_H_

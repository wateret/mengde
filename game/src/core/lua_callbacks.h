#ifndef MENGDE_CORE_LUA_CALLBACKS_H_
#define MENGDE_CORE_LUA_CALLBACKS_H_

#include "lua/lua.h"
#include "lua/ref.h"

namespace mengde {
namespace core {

class LuaCallbacks {
 public:
  LuaCallbacks(lua::Lua* lua) : lua_{lua} {}

 public:
  void end_condition(const lua::Ref& ref) { SetRef(end_condition_, ref); }
  const lua::Ref& end_condition() const { return end_condition_; }
  void on_deploy(const lua::Ref& ref) { SetRef(on_deploy_, ref); }
  const lua::Ref& on_deploy() const { return on_deploy_; }

 private:
  void SetRef(lua::Ref& ref, const lua::Ref& new_ref);

 private:
  lua::Lua* lua_;
  lua::Ref on_deploy_;
  lua::Ref end_condition_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_LUA_CALLBACKS_H_

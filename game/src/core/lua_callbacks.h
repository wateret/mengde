#ifndef MENGDE_CORE_LUA_CALLBACKS_H_
#define MENGDE_CORE_LUA_CALLBACKS_H_

#include <unordered_map>

#include "lua/lua.h"
#include "lua/ref.h"

namespace mengde {
namespace core {

class Stage;

struct EventCallback {
  lua::Ref condition;
  lua::Ref handler;
};

class LuaCallbacks {
 public:
  LuaCallbacks(lua::Lua* lua) : lua_{lua}, next_event_id_{0u} {}

 public:
  void end_condition(const lua::Ref& ref) { SetRef(end_condition_, ref); }
  const lua::Ref& end_condition() const { return end_condition_; }
  void on_deploy(const lua::Ref& ref) { SetRef(on_deploy_, ref); }
  const lua::Ref& on_deploy() const { return on_deploy_; }
  void on_begin(const lua::Ref& ref) { SetRef(on_begin_, ref); }
  const lua::Ref& on_begin() const { return on_begin_; }
  void on_victory(const lua::Ref& ref) { SetRef(on_victory_, ref); }
  const lua::Ref& on_victory() const { return on_victory_; }
  void on_defeat(const lua::Ref& ref) { SetRef(on_defeat_, ref); }
  const lua::Ref& on_defeat() const { return on_defeat_; }

  uint32_t RegisterEvent(const lua::Ref& condition, const lua::Ref& handler);
  void UnregisterEvent(uint32_t id);
  void RunEvents(const lua::LuaClass& stage);

 private:
  void SetRef(lua::Ref& ref, const lua::Ref& new_ref);

 private:
  lua::Lua* lua_;
  lua::Ref on_deploy_;
  lua::Ref on_begin_;
  lua::Ref on_victory_;
  lua::Ref on_defeat_;
  lua::Ref end_condition_;
  std::unordered_map<uint32_t, EventCallback> events_;
  uint32_t next_event_id_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_LUA_CALLBACKS_H_

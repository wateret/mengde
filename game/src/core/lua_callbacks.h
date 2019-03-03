#ifndef MENGDE_CORE_LUA_CALLBACKS_H_
#define MENGDE_CORE_LUA_CALLBACKS_H_

#include <sol.hpp>
#include <unordered_map>

namespace mengde {
namespace core {

class LuaGame;

struct EventCallback {
  sol::function predicate;
  sol::function handler;
};

class LuaCallbacks {
 public:
  LuaCallbacks() : next_event_id_{0u} {}

 public:
  void end_condition(const sol::function& fn) { SetFn(end_condition_, fn); }
  const sol::function& end_condition() const { return end_condition_; }
  void on_deploy(const sol::function& fn) { SetFn(on_deploy_, fn); }
  const sol::function& on_deploy() const { return on_deploy_; }
  void on_begin(const sol::function& fn) { SetFn(on_begin_, fn); }
  const sol::function& on_begin() const { return on_begin_; }
  void on_victory(const sol::function& fn) { SetFn(on_victory_, fn); }
  const sol::function& on_victory() const { return on_victory_; }
  void on_defeat(const sol::function& fn) { SetFn(on_defeat_, fn); }
  const sol::function& on_defeat() const { return on_defeat_; }

  uint32_t RegisterEvent(const sol::function& predicate, const sol::function& handler);
  void UnregisterEvent(uint32_t id);
  void RunEvents(LuaGame& lua_game);

 private:
  void SetFn(sol::function& fn, const sol::function& new_fn);

 private:
  sol::function on_deploy_;
  sol::function on_begin_;
  sol::function on_victory_;
  sol::function on_defeat_;
  sol::function end_condition_;
  std::unordered_map<uint32_t, EventCallback> events_;
  uint32_t next_event_id_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_LUA_CALLBACKS_H_

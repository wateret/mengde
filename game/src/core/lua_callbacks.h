#ifndef MENGDE_CORE_LUA_CALLBACKS_H_
#define MENGDE_CORE_LUA_CALLBACKS_H_

#include <unordered_map>

#include "luab/lua.h"
#include "luab/ref.h"

namespace mengde {
namespace core {

class Stage;

struct EventCallback {
  luab::Ref condition;
  luab::Ref handler;
};

class LuaCallbacks {
 public:
  LuaCallbacks(luab::Lua* lua) : lua_{lua}, next_event_id_{0u} {}

 public:
  void end_condition(const luab::Ref& ref) { SetRef(end_condition_, ref); }
  const luab::Ref& end_condition() const { return end_condition_; }
  void on_deploy(const luab::Ref& ref) { SetRef(on_deploy_, ref); }
  const luab::Ref& on_deploy() const { return on_deploy_; }
  void on_begin(const luab::Ref& ref) { SetRef(on_begin_, ref); }
  const luab::Ref& on_begin() const { return on_begin_; }
  void on_victory(const luab::Ref& ref) { SetRef(on_victory_, ref); }
  const luab::Ref& on_victory() const { return on_victory_; }
  void on_defeat(const luab::Ref& ref) { SetRef(on_defeat_, ref); }
  const luab::Ref& on_defeat() const { return on_defeat_; }

  uint32_t RegisterEvent(const luab::Ref& condition, const luab::Ref& handler);
  void UnregisterEvent(uint32_t id);
  void RunEvents(const luab::LuaClass& stage);

 private:
  void SetRef(luab::Ref& ref, const luab::Ref& new_ref);

 private:
  luab::Lua* lua_;
  luab::Ref on_deploy_;
  luab::Ref on_begin_;
  luab::Ref on_victory_;
  luab::Ref on_defeat_;
  luab::Ref end_condition_;
  std::unordered_map<uint32_t, EventCallback> events_;
  uint32_t next_event_id_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_LUA_CALLBACKS_H_

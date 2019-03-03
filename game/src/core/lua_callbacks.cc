#include "lua_callbacks.h"

#include "lua_game.h"
#include "util/logger.h"

namespace mengde {
namespace core {

void LuaCallbacks::SetFn(sol::function& fn, const sol::function& new_fn) { fn = new_fn; }

uint32_t LuaCallbacks::RegisterEvent(const sol::function& predicate, const sol::function& handler) {
  auto id = next_event_id_++;
  assert(events_.find(id) == events_.end());
  events_[id] = {predicate, handler};
  return id;
}

void LuaCallbacks::UnregisterEvent(uint32_t id) {
  auto found = events_.find(id);
  if (found == events_.end()) {
    LOG_WARNING("Tried to unregister the event that is not exist.");
  } else {
    events_.erase(found);
  }
}

void LuaCallbacks::RunEvents(LuaGame& lua_game) {
  for (const auto& e : events_) {
    auto id = e.first;
    auto cb = e.second;
    auto matched = cb.predicate(lua_game);
    if (matched) {
      cb.handler(lua_game, id);
    }
  }
}

}  // namespace core
}  // namespace mengde

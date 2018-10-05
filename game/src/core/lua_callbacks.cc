#include "lua_callbacks.h"

#include "stage.h"
#include "util/logger.h"

namespace mengde {
namespace core {

void LuaCallbacks::SetRef(luab::Ref& ref, const luab::Ref& new_ref) {
  if (!ref.nil()) {
    lua_->UnRef(ref.value());
  }
  ref = new_ref;
}

uint32_t LuaCallbacks::RegisterEvent(const luab::Ref& condition, const luab::Ref& handler) {
  auto id = next_event_id_++;
  assert(events_.find(id) == events_.end());
  events_.insert({id, {condition, handler}});
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

void LuaCallbacks::RunEvents(const luab::LuaClass& stage) {
  for (const auto& e : events_) {
    auto id = e.first;
    auto cb = e.second;
    auto matched = lua_->Call<bool>(cb.condition, stage);
    if (matched) {
      lua_->Call<void>(cb.handler, stage, id);
    }
  }
}

}  // namespace core
}  // namespace mengde

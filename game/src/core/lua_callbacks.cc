#include "lua_callbacks.h"

#include "stage.h"

namespace mengde {
namespace core {

void LuaCallbacks::SetRef(lua::Ref& ref, const lua::Ref& new_ref) {
  if (!ref.nil()) {
    lua_->UnRef(ref.value());
  }
  ref = new_ref;
}

uint32_t LuaCallbacks::RegisterEvent(const lua::Ref& condition, const lua::Ref& handler) {
  auto id = static_cast<uint32_t>(events_.size());
  events_.push_back({condition, handler});
  return id;
}

void LuaCallbacks::RunEvents(const lua::LuaClass& stage) {
  for (const auto& e : events_) {
    auto matched = lua_->Call<bool>(e.condition, stage);
    if (matched) {
      lua_->Call<void>(e.handler, stage);
    }
  }
}

}  // namespace core
}  // namespace mengde

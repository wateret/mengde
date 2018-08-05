#include "lua_callbacks.h"

namespace mengde {
namespace core {

void LuaCallbacks::SetRef(lua::Ref& ref, const lua::Ref& new_ref) {
  if (!ref.nil()) {
    lua_->UnRef(ref.value());
  }
  ref = new_ref;
}

}  // namespace core
}  // namespace mengde

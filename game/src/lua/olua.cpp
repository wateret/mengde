#include "olua.h"

namespace lua {

Olua::Olua() : Lua() {
}

Olua::Olua(lua_State* L) : Lua(L) {
}

} // namespace lua

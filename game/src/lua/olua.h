#ifndef O_LUA_H_
#define O_LUA_H_

#include "lua.h"

namespace lua {

class Olua : public Lua {
 public:
  Olua();
  Olua(lua_State* L);

 public:
};

} // namespace lua

#endif // O_LUA_H_

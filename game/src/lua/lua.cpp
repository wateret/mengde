#include "lua.h"

#include <stdio.h>

namespace lua {

Lua::Lua() : L(nullptr), destroy_(true) {
  L = luaL_newstate();
  if (L != nullptr) luaL_openlibs(L);
}

Lua::Lua(lua_State* L) : L(L), destroy_(false) {
}

Lua::~Lua() {
	if (L != nullptr && destroy_) lua_close(L);
}

void Lua::Run(const string& filename) {
  if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
    printf("LuaError: Load and run '%s' failed. \n", filename.c_str());
  }
}

void Lua::Register(const string& name, lua_CFunction fn) {
  lua_register(L, name.c_str(), fn);
}

void Lua::LogError(const string& msg) {
  DumpStack();
  LOGM_ERROR(Lua, "%s", msg.c_str());
}

void Lua::LogWarning(const string& msg) {
  LOGM_WARNING(Lua, "%s", msg.c_str());
}

void Lua::LogDebug(const string& msg) {
  LOGM_DEBUG(Lua, "%s", msg.c_str());
}

void Lua::ForEachTableEntry(const string& name, ForEachEntryFunc cb) {
  int num_stack = GetToStack(name);
  if (!lua_istable(L, -1)) { // Table not found
    return;
  }
  lua_pushnil(L);
  // TODO handle when table has no entry
  while (lua_next(L, -2)) {
    cb();
    lua_pop(L, 1);
  }
  PopStack(num_stack); // Pop tables pushed by GetToStack
}

void Lua::PushToStack(const string& s) {
  lua_pushstring(L, s.c_str());
}

void Lua::PushToStack(lua_CFunction fn) {
  lua_pushcfunction(L, fn);
}

void Lua::SetGlobal(const string& name, const string& val) {
  lua_pushstring(L, val.c_str());
  lua_setglobal(L, name.c_str());
}

void Lua::GetField(const string& id) {
  if (GetStackSize() == 0) {
    lua_getglobal(L, id.c_str());
  } else {
    assert(GetStackSize() > 0);
    // At this point Lua Stack must be
    // Index -1 : value
    lua_getfield(L, -1, id.c_str());
  }
}

void Lua::SetField(const string& id) {
  if (GetStackSize() == 1) {
    lua_setglobal(L, id.c_str());
  } else {
    assert(GetStackSize() > 1);
    // At this point Lua Stack must be look like
    // Index -1 : value (to be set)
    // Index -2 : table (to set a field)
    // So the below lua_setfield() does `table[id] = value`
    lua_setfield(L, -2, id.c_str());
  }
}

int Lua::GetStackSize() {
  return lua_gettop(L);
}

void Lua::DumpStack() {
#ifdef DEBUG
  int i = lua_gettop(L);
  printf("--------------- Stack Dump Begin ----------------\n");
  while (i) {
    int t = lua_type(L, i);
    switch (t) {
      case LUA_TSTRING:
        printf("%d: \"%s\"", i, lua_tostring(L, i));
        break;
      case LUA_TBOOLEAN:
        printf("%d: %s", i, lua_toboolean(L, i) ? "true" : "false");
        break;
      case LUA_TNUMBER:
        printf("%d: %g", i, lua_tonumber(L, i));
        break;
      default:
        printf("%d: %s", i, lua_typename(L, t));
        break;
    }
    printf("\n");
    i--;
  }
  printf("--------------- Stack Dump End   ----------------\n");
#endif // DEBUG
}

template<>
void Lua::CallImpl<void>(unsigned argc) {
  if (lua_pcall(L, argc, 0, 0)) {
    LogError("Error on Call(return type : void).");
  }
}

void Lua::NewGlobalTable(const string& name) {
  lua_newtable(L);
  lua_setglobal(L, name.c_str());
}

} // namespace lua

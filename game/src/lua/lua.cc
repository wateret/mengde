#include "lua.h"

#include <stdio.h>

#include <regex>

namespace {

std::string string_replace_all(const std::string& str, const std::string& from, const std::string& to) {
  return std::regex_replace(str, std::regex(from), to);
}

}  // namespace

namespace lua {

Lua::Lua() : L(nullptr), destroy_(true) {
  L = luaL_newstate();
  if (L != nullptr) luaL_openlibs(L);
}

Lua::Lua(lua_State* L) : L(L), destroy_(false) {}

Lua::~Lua() {
  if (L != nullptr && destroy_) lua_close(L);
}

void Lua::RunFile(const std::string& filename) {
  if (auto code = luaL_loadfile(L, filename.c_str())) {
    if (code == LUA_ERRSYNTAX) {
      auto s = std::string(lua_tostring(L, -1));
      throw ScriptSyntaxException{s};
    } else {
      throw ScriptRuntimeException{std::string("Unknown error while running script.")};
    }
  }
  if (lua_pcall(L, 0, 0, 0)) {
    std::string message(lua_tostring(L, -1));
    throw ScriptRuntimeException(message);
  }
}

void Lua::RunScript(const std::string& code) {
  if (luaL_loadstring(L, code.c_str())) {
    // TODO Dump lua error (the std::string on the stack top)
    throw "Loadfile error";
  }
  if (lua_pcall(L, 0, 0, 0)) {
    std::string message(lua_tostring(L, -1));
    throw ScriptRuntimeException(message);
  }
}

void Lua::Register(const std::string& name, lua_CFunction fn) { lua_register(L, name.c_str(), fn); }

void Lua::LogError(const std::string&) {
  DumpStack();
  //  LOGM_ERROR(Lua, "%s", msg.c_str());
}

void Lua::LogWarning(const std::string&) {
  //  LOGM_WARNING(Lua, "%s", msg.c_str());
}

void Lua::LogDebug(const std::string&) {
  //  LOGM_DEBUG(Lua, "%s", msg.c_str());
}

void Lua::ForEachTableEntry(const std::string& name, ForEachEntryFunc cb) {
  int num_stack = GetToStack(name);
  if (!lua_istable(L, -1)) {  // Table not found
    return;
  }

  lua_pushnil(L);
  while (lua_next(L, -2)) {
    // Lua can have any type for table key, but we support only std::string keys.
    std::string key = "";
    if (!lua_isnumber(L, -2) && lua_isstring(L, -2)) key = lua_tostring(L, -2);
    cb(this, key);
    lua_pop(L, 1);
  }
  PopStack(num_stack);  // Pop tables pushed by GetToStack
}

void Lua::PushToStack(const std::string& s) { lua_pushstring(L, s.c_str()); }

void Lua::PushToStack(lua_CFunction fn) { lua_pushcfunction(L, fn); }

void Lua::PushToStack(const LuaClass& object) {
  // Specially handle LuaClass objects
  // Call metatable object for object construction)
  lua_getglobal(L, object.name().c_str());
  if (lua_pcall(L, 0, 1, 0)) {
    LogError("Error on Call");
  }
  // Set C object field
  Set("__cobj", object.pointer());
}

void Lua::SetGlobal(const std::string& name, const std::string& val) {
  lua_pushstring(L, val.c_str());
  lua_setglobal(L, name.c_str());
}

void Lua::GetField(const std::string& id) {
  if (GetStackSize() == 0) {
    lua_getglobal(L, id.c_str());
  } else {
    assert(GetStackSize() > 0);
    // At this point Lua Stack must be
    // Index -1 : value
    lua_getfield(L, -1, id.c_str());
  }
}

void Lua::SetField(const std::string& id) {
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

void Lua::RegisterClass(const std::string& class_name) {
  static const char tpl[] =
      "\
<C> = {} \n\
<C>.__index = <C>\n\
\n\
setmetatable(<C>, {\n\
  __call = function (cls, ...)\n\
    return cls.new(...)\n\
  end,\n\
})\n\
\n\
function <C>.new(cobj)\n\
  local self = setmetatable({}, <C>)\n\
  self.__cobj = cobj\n\
  return self\n\
end\n\
";
  std::string code = string_replace_all(tpl, "<C>", class_name);
  RunScript(code);
}

void Lua::RegisterMethod(const std::string& class_name, const std::string& method_name) {
  static const char tpl[] =
      "\
function <C>:<M>(...)\n\
  return <C>_<M>(self.__cobj, ...)\n\
end\n\
";
  std::string code = string_replace_all(tpl, "<C>", class_name);
  code = string_replace_all(code, "<M>", method_name);
  RunScript(code);
}

int Lua::GetStackSize() { return lua_gettop(L); }

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
#endif  // DEBUG
}

template <>
void Lua::CallImpl<void>(unsigned argc) {
  if (lua_pcall(L, argc, 0, 0)) {
    LogError("Error on Call(return type : void).");
  }
}

void Lua::NewGlobalTable(const std::string& name) {
  lua_newtable(L);
  lua_setglobal(L, name.c_str());
}

}  // namespace lua

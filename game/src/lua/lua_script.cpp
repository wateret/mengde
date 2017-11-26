#include "lua_script.h"
#include <stdio.h>

LuaScript::LuaScript() : L(nullptr), destroy_(true) {
  L = luaL_newstate();
  if (L != nullptr) luaL_openlibs(L);
}

LuaScript::LuaScript(lua_State* L) : L(L), destroy_(false) {
}

LuaScript::~LuaScript() {
	if (L != nullptr && destroy_) lua_close(L);
}

void LuaScript::Run(const string& filename) {
  if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
    printf("LuaError: Load and run '%s' failed. \n", filename.c_str());
  }
}

void LuaScript::Register(const string& name, lua_CFunction fn) {
  lua_register(L, name.c_str(), fn);
}

void LuaScript::PrintError(const string& variableName, const string& reason) {
  printf("[Lua][Error] Cannot get \"%s\". %s\n", variableName.c_str(), reason.c_str());
}

void LuaScript::ForEachTableEntry(const string& name, ForEachEntryFunc cb) {
  int num_stack = GetToStack(name.c_str());
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

void LuaScript::PushToStack(const string& s) {
  lua_pushstring(L, s.c_str());
}

void LuaScript::PushToStack(lua_CFunction fn) {
  lua_pushcfunction(L, fn);
}

void LuaScript::SetRawPointerToGlobal(const string& name, void* p) {
  lua_pushlightuserdata(L, p);
  lua_setglobal(L, name.c_str());
}

void LuaScript::SetGlobal(const string& name, const string& val) {
  lua_pushstring(L, val.c_str());
  lua_setglobal(L, name.c_str());
}

template<>
void LuaScript::Call<void>(unsigned argc) {
  if (lua_pcall(L, argc, 0, 0)) {
    PrintError("", "Error on Call");
  }
}

void LuaScript::NewGlobalTable(const string& name) {
  lua_newtable(L);
  lua_setglobal(L, name.c_str());
}

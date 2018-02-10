#ifndef LUA_SCRIPT_H_
#define LUA_SCRIPT_H_

#include <string>
#include <vector>
#include <functional>

#include "util/common.h"
#include "table.h"

extern "C" {

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

}

namespace lua {

class LuaClass {
 public:
  template <typename T>
  LuaClass(T* pointer, const std::string& name) : pointer_(static_cast<void*>(pointer)), name_(name) {}

  void* pointer() const { return pointer_; }
  const std::string& name() const { return name_; }

 private:
  void* pointer_;
  std::string name_;
};

class Lua {
 public:
  typedef function<void(Lua* lua, const std::string& key)> ForEachEntryFunc;
  Lua();
  Lua(lua_State*);
  virtual ~Lua();

  void ForEachTableEntry(const string&, ForEachEntryFunc);
  void Run(const string& filename);
  void RunScript(const string& code);
  void Register(const string& name, lua_CFunction);

  void PopStack(unsigned n) {
    lua_pop(L, n);
  }

  // Call a function with variadic template
  template<typename R, typename... Args>
  R Call(const string& name, Args... args) {
    GetToStack(name); // XXX should pop more when name is nested
    return CallImpl<R>(0, args...);
  }

  template<typename T>
  vector<T> GetVector(const string& name = "") {
    vector<T> vec;
    ForEachTableEntry(name, [&] (Lua* lua, const std::string&) {
      T val = lua->GetTop<T>();
      vec.push_back(val);
    });
    // FIXME pop or not?
    return vec;
  }

  // Get a required entry
  // If the entry is not exist, emits an error.
  template<typename T>
  T Get(const string& var_expr = "") {
    ASSERT(L != nullptr);

    int to_be_popped = GetToStack(var_expr);
    T result = GetTop<T>();

    PopStack(to_be_popped);

    return result;
  }

  // Get an optional entry
  // If the entry is not exist, returns default value.
  template<typename T>
  T GetOpt(const string& var_expr = "") {
    ASSERT(L != nullptr);

    int to_be_popped = GetToStackOpt(var_expr);
    T result = GetTopOpt<T>();

    PopStack(to_be_popped);

    return result;
  }

  // Get a required entry and remove it from stack
  template<typename T>
  T Pop() {
    T result = Get<T>();
    PopStack(1);
    return result;
  }

  template<typename T>
  void SetGlobal(const string& name, T val) {
    lua_pushnumber(L, (double)val);
    lua_setglobal(L, name.c_str());
  }

  void SetGlobal(const string&, const string&);

  void NewGlobalTable(const string&);

  template<typename T>
  void Set(const string& var_expr, T val) {
    int initial_stack_size = GetStackSize();

    string var = "";
    int level = 0;
    for (unsigned int i = 0, size = var_expr.size(); i < size; i++) {
      if (var_expr[i] == '.') { // Handle a var name in the middle
        // Find field
        GetField(var);
        if (lua_isnil(L, -1)) { // Field not found
          // Create a new table
          lua_pop(L, 1);
          lua_newtable(L);
          SetField(var);
          GetField(var);
        }
        var = "";
        level++;
      } else {
        var += var_expr[i];
      }
    }
    // Handle the last var name - Set a field
    PushToStack(val);
    SetField(var);
    lua_pop(L, level);

    ASSERT(initial_stack_size == GetStackSize());
  }

  // PushToStack is public for pushing return value from lua C function
  // and it is also used frequently internally.

  template<typename T>
  void PushToStack(T val) {
    lua_pushnumber(L, (double)val);
  }

  template<typename T>
  void PushToStack(T* val) {
    lua_pushlightuserdata(L, static_cast<void*>(val));
  }

  void PushToStack(const string& s);
  void PushToStack(lua_CFunction fn);
  void PushToStack(const LuaClass& object);

  // OO-style registering class and method

  void RegisterClass(const string& class_name);
  void RegisterMethod(const string& class_name, const string& method_name);

  // For debugging

  void DumpStack();

 protected:

  int GetToStack(const string& var_expr, bool optional = false) {
    if (var_expr.size() == 0) return 0;

    string var = "";
    int level = 0;
    for (unsigned int i = 0, size = var_expr.size(); i < size + 1; i++) {
      if (i == size || var_expr[i] == '.') {
        GetField(var);
        level++;
        if (lua_isnil(L, -1)) {
          // Cleanup and return or throw
          PopStack(level);
          if (!optional) {
            LogError(var + " is not defined");
            throw "GetToStack Error";
          }
          return 0;
        }
        var = "";
      } else {
        var += var_expr[i];
      }
    }

    return level;
  }

  int GetToStackOpt(const string& var_expr) {
    return GetToStack(var_expr, true);
  }

  // type aliases

  template<typename T> using is_bool   = std::is_same<bool,   T>;
  template<typename T> using is_string = std::is_same<string, T>;
  template<typename T> using is_table  = std::is_same<Table*, T>;

  template<typename T> struct is_vector { static const bool value = false; };
  template<typename T> struct is_vector<std::vector<T>> { static const bool value = true; };

  // Arithmetic types except bool

  template<typename T>
  typename std::enable_if<std::is_arithmetic<T>::value && !is_bool<T>::value, T>::type GetDefault() {
    return static_cast<T>(0);
  }

  template<typename T>
  typename std::enable_if<std::is_arithmetic<T>::value && !is_bool<T>::value, T>::type GetTop() {
    if (!lua_isnumber(L, -1)) {
      LogError("Not a number.");
      throw "Not a number";
    }
    return (T)lua_tonumber(L, -1);
  }

  template<typename T>
  typename std::enable_if<std::is_arithmetic<T>::value && !is_bool<T>::value, T>::type GetTopOpt() {
    if (!lua_isnumber(L, -1)) {
      LogDebug("Not a number. Returning default value.");
      return GetDefault<T>();
    }
    return (T)lua_tonumber(L, -1);
  }

  // bool

  template<typename T>
  typename std::enable_if<is_bool<T>::value, T>::type GetDefault() {
    return false;
  }

  template<typename T>
  typename std::enable_if<is_bool<T>::value, T>::type GetTop() {
    if (lua_isboolean(L, -1)) {
      return (bool)lua_toboolean(L, -1);
    } else {
      LogError("Not a boolean.");
      throw "Not a boolean";
    }
  }

  template<typename T>
  typename std::enable_if<is_bool<T>::value, T>::type GetTopOpt() {
    if (lua_isboolean(L, -1)) {
      return (bool)lua_toboolean(L, -1);
    } else {
      return GetDefault<bool>();
    }
  }

  // string

  template<typename T>
  typename std::enable_if<is_string<T>::value, T>::type GetDefault() {
    return "nil";
  }

  template<typename T>
  typename std::enable_if<is_string<T>::value, T>::type GetTop() {
    if (lua_isstring(L, -1)) {
      return string(lua_tostring(L, -1));
    } else {
      LogError("Not a string.");
      throw "Not a string";
    }
  }

  template<typename T>
  typename std::enable_if<is_string<T>::value, T>::type GetTopOpt() {
    if (lua_isstring(L, -1)) {
      return string(lua_tostring(L, -1));
    } else {
      return GetDefault<string>();
    }
  }

  // Pointer types

  template<typename T>
  typename std::enable_if<std::is_pointer<T>::value && !is_table<T>::value, T>::type GetDefault() { return nullptr; }

  template<typename T>
  typename std::enable_if<std::is_pointer<T>::value && !is_table<T>::value, T>::type GetTop() {
    if (lua_islightuserdata(L, -1)) {
      return reinterpret_cast<T>(lua_touserdata(L, -1));
    } else {
      LogError("Not a pointer.");
      throw "Not a pointer";
    }
  }

  template<typename T>
  typename std::enable_if<std::is_pointer<T>::value && !is_table<T>::value, T>::type GetTopOpt() {
    if (lua_islightuserdata(L, -1)) {
      return reinterpret_cast<T>(lua_touserdata(L, -1));
    } else {
      return GetDefault<T>();
    }
  }

  // lua::Table type

  template<typename T>
  typename std::enable_if<is_table<T>::value, T>::type GetDefault() { return nullptr; }

  template<typename T>
  typename std::enable_if<is_table<T>::value, T>::type GetTop() {
    Table* table = new Table();
    ForEachTableEntry("", [&] (Lua*, const std::string& key) {
      if (lua_istable(L, -1)) {
        table->Add(key, Get<Table*>());
      } else if (lua_isnumber(L, -1)) {
        // FIXME handle double and other numeric types
        table->Add(key, Get<int32_t>());
      } else if (lua_isstring(L, -1)) {
        table->Add(key, Get<std::string>());
      } else if (lua_isuserdata(L, -1)) {
        table->Add(key, Get<void*>());
      } else {
        assert(false && "Unsupported type.");
      }
    });
    return table;
  }

  /*
  template<typename T>
  typename std::enable_if<is_table<T>::value, T>::type GetTopOpt() {
  }
  */

  // Vector types

  template<typename T>
  typename std::enable_if<is_vector<T>::value, T>::type GetDefault() {
    return T(); // Return default vector type
  }

  template<typename T>
  typename std::enable_if<is_vector<T>::value, T>::type GetTop() {
    T vec;
    ForEachTableEntry("", [&] (Lua* lua, const std::string&) {
      typename T::value_type val = lua->GetTop<typename T::value_type>();
      vec.push_back(val);
    });
    return vec;
  }

  /*
  template<typename T>
  typename std::enable_if<is_vector<T>::value, T>::type GetTopOpt() {
    if (lua_isstring(L, -1)) {
      return string(lua_tostring(L, -1));
    } else {
      return GetDefault<string>();
    }
  }
  */

  void LogError(const string&);
  void LogWarning(const string&);
  void LogDebug(const string&);

  void GetField(const string& id);
  void SetField(const string& id);
  int  GetStackSize();

  template<typename R>
  R CallImpl(unsigned argc) {
    if (lua_pcall(L, argc, 1, 0)) {
      LogError("Error on Call");
    }
    R ret = GetTop<R>();
    PopStack(1);
    return ret;
  }

  template<typename R, typename A, typename... Args>
  R CallImpl(unsigned argc, A arg0, Args... args) {
    PushToStack(arg0);
    return CallImpl<R>(argc + 1, args...);
  }

 protected:
  lua_State* L;
  bool       destroy_;
};

//
// Template Specializations
//

// method CallImpl - for return type of `void`
template<>
void Lua::CallImpl<void>(unsigned argc);

} // namespace lua

#endif // LUA_SCRIPT_H_

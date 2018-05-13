#ifndef LUA_LUA_H_
#define LUA_LUA_H_

#include <cassert>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "table.h"

extern "C" {

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

namespace lua {

class LuaException : public std::exception {
 public:
  LuaException() : std::exception() {}
};

class UndeclaredVariableException : public LuaException {
 public:
  UndeclaredVariableException(const std::string& var_expr) : LuaException(), var_expr_(var_expr) {}
  virtual const char* what() const throw() { return (var_expr_ + " is undeclared.").c_str(); }

 private:
  std::string var_expr_;
};

class WrongTypeException : public LuaException {
 public:
  WrongTypeException(const std::string& type_expected, const std::string& type_actual)
      : LuaException(), type_expected_(type_expected), type_actual_(type_actual) {}
  virtual const char* what() const throw() {
    return ("Wrong type - (Expected: " + type_expected_ + ", Actual: " + type_actual_ + ")").c_str();
  }

 private:
  std::string type_expected_;
  std::string type_actual_;
};

class ScriptRuntimeException : public LuaException {
 public:
  ScriptRuntimeException(const std::string& message) : LuaException(), message_(message) {}
  virtual const char* what() const throw() { return message_.c_str(); }

 private:
  std::string message_;
};

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
  typedef std::function<void(Lua* lua, const std::string& key)> ForEachEntryFunc;
  Lua();
  Lua(lua_State*);
  virtual ~Lua();

  void ForEachTableEntry(const std::string&, ForEachEntryFunc);
  void RunFile(const std::string& filename);
  void RunScript(const std::string& code);
  void Register(const std::string& name, lua_CFunction);

  void PopStack(unsigned n) { lua_pop(L, n); }

  // Call a function with variadic template
  template <typename R, typename... Args>
  R Call(const std::string& name, Args... args) {
    GetToStack(name);  // XXX should pop more when name is nested
    return CallImpl<R>(0, args...);
  }

  template <typename T>
  std::vector<T> GetVector(const std::string& name = "") {
    std::vector<T> vec;
    ForEachTableEntry(name, [&](Lua* lua, const std::string&) {
      T val = lua->GetTop<T>();
      vec.push_back(val);
    });
    // FIXME pop or not?
    return vec;
  }

  // Get a required entry
  // If the entry is not exist, emits an error.
  template <typename T>
  T Get(const std::string& var_expr = "") {
    assert(L != nullptr);

    int to_be_popped = GetToStack(var_expr);
    T result = GetTop<T>();

    PopStack(to_be_popped);

    return result;
  }

  // Get an optional entry
  // If the entry is not exist, returns default value.
  template <typename T>
  T GetOpt(const std::string& var_expr = "") {
    assert(L != nullptr);

    int to_be_popped = GetToStackOpt(var_expr);
    T result = GetTopOpt<T>();

    PopStack(to_be_popped);

    return result;
  }

  // Get a required entry and remove it from stack
  template <typename T>
  T Pop() {
    T result = Get<T>();
    PopStack(1);
    return result;
  }

  template <typename T>
  void SetGlobal(const std::string& name, T val) {
    lua_pushnumber(L, (double)val);
    lua_setglobal(L, name.c_str());
  }

  void SetGlobal(const std::string&, const std::string&);

  void NewGlobalTable(const std::string&);

  template <typename T>
  void Set(const std::string& var_expr, T val) {
#ifdef DEBUG
    int initial_stack_size = GetStackSize();
#endif

    std::string var = "";
    int level = 0;
    for (unsigned int i = 0, size = var_expr.size(); i < size; i++) {
      if (var_expr[i] == '.') {  // Handle a var name in the middle
        // Find field
        GetField(var);
        if (lua_isnil(L, -1)) {  // Field not found
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

#ifdef DEBUG
    assert(initial_stack_size == GetStackSize());
#endif
  }

  // PushToStack is public for pushing return value from lua C function
  // and it is also used frequently internally.

  template <typename T>
  void PushToStack(T val) {
    lua_pushnumber(L, (double)val);
  }

  template <typename T>
  void PushToStack(T* val) {
    lua_pushlightuserdata(L, static_cast<void*>(val));
  }

  void PushToStack(const std::string& s);
  void PushToStack(lua_CFunction fn);
  void PushToStack(const LuaClass& object);

  // OO-style registering class and method

  void RegisterClass(const std::string& class_name);
  void RegisterMethod(const std::string& class_name, const std::string& method_name);

  // For debugging

  void DumpStack();

 protected:
  int GetToStack(const std::string& var_expr, bool optional = false) {
    if (var_expr.size() == 0) return 0;

    std::string var = "";
    int level = 0;
    for (unsigned int i = 0, size = var_expr.size(); i < size + 1; i++) {
      if (i == size || var_expr[i] == '.') {
        GetField(var);
        level++;
        if (lua_isnil(L, -1)) {
          // Cleanup and return or throw
          PopStack(level);
          if (!optional) {
            throw UndeclaredVariableException(var_expr);
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

  int GetToStackOpt(const std::string& var_expr) { return GetToStack(var_expr, true); }

  // type aliases

  template <typename T>
  using is_bool = std::is_same<bool, T>;
  template <typename T>
  using is_string = std::is_same<std::string, T>;
  template <typename T>
  using is_table = std::is_same<Table*, T>;

  template <typename T>
  struct is_vector {
    static const bool value = false;
  };
  template <typename T>
  struct is_vector<std::vector<T>> {
    static const bool value = true;
  };

  // Arithmetic types except bool

  template <typename T>
  typename std::enable_if<std::is_arithmetic<T>::value && !is_bool<T>::value, T>::type GetDefault() {
    return static_cast<T>(0);
  }

  template <typename T>
  typename std::enable_if<std::is_arithmetic<T>::value && !is_bool<T>::value, T>::type GetTop() {
    if (!lua_isnumber(L, -1)) {
      throw WrongTypeException("number", "?");
    }
    return (T)lua_tonumber(L, -1);
  }

  template <typename T>
  typename std::enable_if<std::is_arithmetic<T>::value && !is_bool<T>::value, T>::type GetTopOpt() {
    if (!lua_isnumber(L, -1)) {
      LogDebug("Not a number. Returning default value.");
      return GetDefault<T>();
    }
    return (T)lua_tonumber(L, -1);
  }

  // bool

  template <typename T>
  typename std::enable_if<is_bool<T>::value, T>::type GetDefault() {
    return false;
  }

  template <typename T>
  typename std::enable_if<is_bool<T>::value, T>::type GetTop() {
    if (lua_isboolean(L, -1)) {
      return (bool)lua_toboolean(L, -1);
    } else {
      throw WrongTypeException("boolean", "?");
    }
  }

  template <typename T>
  typename std::enable_if<is_bool<T>::value, T>::type GetTopOpt() {
    if (lua_isboolean(L, -1)) {
      return (bool)lua_toboolean(L, -1);
    } else {
      return GetDefault<bool>();
    }
  }

  // std::string

  template <typename T>
  typename std::enable_if<is_string<T>::value, T>::type GetDefault() {
    return "nil";
  }

  template <typename T>
  typename std::enable_if<is_string<T>::value, T>::type GetTop() {
    if (lua_isstring(L, -1)) {
      return std::string(lua_tostring(L, -1));
    } else {
      throw WrongTypeException("std::string", "?");
    }
  }

  template <typename T>
  typename std::enable_if<is_string<T>::value, T>::type GetTopOpt() {
    if (lua_isstring(L, -1)) {
      return std::string(lua_tostring(L, -1));
    } else {
      return GetDefault<std::string>();
    }
  }

  // Pointer types

  template <typename T>
  typename std::enable_if<std::is_pointer<T>::value && !is_table<T>::value, T>::type GetDefault() {
    return nullptr;
  }

  template <typename T>
  typename std::enable_if<std::is_pointer<T>::value && !is_table<T>::value, T>::type GetTop() {
    if (lua_islightuserdata(L, -1)) {
      return reinterpret_cast<T>(lua_touserdata(L, -1));
    } else {
      throw WrongTypeException("pointer", "?");
    }
  }

  template <typename T>
  typename std::enable_if<std::is_pointer<T>::value && !is_table<T>::value, T>::type GetTopOpt() {
    if (lua_islightuserdata(L, -1)) {
      return reinterpret_cast<T>(lua_touserdata(L, -1));
    } else {
      return GetDefault<T>();
    }
  }

  // lua::Table type

  template <typename T>
  typename std::enable_if<is_table<T>::value, T>::type GetDefault() {
    return nullptr;
  }

  template <typename T>
  typename std::enable_if<is_table<T>::value, T>::type GetTop() {
    Table* table = new Table();
    ForEachTableEntry("", [&](Lua*, const std::string& key) {
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

  // std::vector types

  template <typename T>
  typename std::enable_if<is_vector<T>::value, T>::type GetDefault() {
    return T();  // Return default std::vector type
  }

  template <typename T>
  typename std::enable_if<is_vector<T>::value, T>::type GetTop() {
    T vec;
    ForEachTableEntry("", [&](Lua* lua, const std::string&) {
      typename T::value_type val = lua->GetTop<typename T::value_type>();
      vec.push_back(val);
    });
    return vec;
  }

  /*
  template<typename T>
  typename std::enable_if<is_vector<T>::value, T>::type GetTopOpt() {
    if (lua_isstring(L, -1)) {
      return std::string(lua_tostring(L, -1));
    } else {
      return GetDefault<std::string>();
    }
  }
  */

  void LogError(const std::string&);
  void LogWarning(const std::string&);
  void LogDebug(const std::string&);

  void GetField(const std::string& id);
  void SetField(const std::string& id);
  int GetStackSize();

  template <typename R>
  R CallImpl(unsigned argc) {
    if (lua_pcall(L, argc, 1, 0)) {
      LogError("Error on Call");
    }
    R ret = GetTop<R>();
    PopStack(1);
    return ret;
  }

  template <typename R, typename A, typename... Args>
  R CallImpl(unsigned argc, A arg0, Args... args) {
    PushToStack(arg0);
    return CallImpl<R>(argc + 1, args...);
  }

 protected:
  lua_State* L;
  bool destroy_;
};

//
// Template Specializations
//

// method CallImpl - for return type of `void`
template <>
void Lua::CallImpl<void>(unsigned argc);

}  // namespace lua

#endif  // LUA_LUA_H_

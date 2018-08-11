#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "lua/lua.h"

BOOST_AUTO_TEST_CASE(CallLuaFuncFromCpp_1) {
  ::lua::Lua l;
  l.RunScript(
      std::string("function foo()\n"
                  "  return 1\n"
                  "end\n"));
  int ret = l.Call<int>(std::string("foo"));
  BOOST_CHECK(ret == 1);
}

BOOST_AUTO_TEST_CASE(CallLuaFuncFromCpp_2) {
  ::lua::Lua l;
  l.RunScript(
      std::string("function add(a, b)\n"
                  "  return a + b\n"
                  "end\n"));
  int ret = l.Call<int>(std::string("add"), 1, 2);
  BOOST_CHECK(ret == 3);
}

extern "C" {

static ::lua::Ref g_ref_1{-1};

static int set_callback(lua_State* L) {
  ::lua::Lua l{L};
  ::lua::Ref ref = l.Pop<::lua::Ref>();
  g_ref_1 = ref;
  return 0;
}
}  // extern "C"

//
// CalLuaCallbackFromCpp
// 1. Register C function to Lua which registers lua::Ref to g_ref
// 2. Call C function from Lua to add a Lua callback
// 3. Call Lua function with g_ref(a lua::Ref) from C++

BOOST_AUTO_TEST_CASE(CallLuaFunctionWithRefFromCpp_1) {
  ::lua::Lua l;
  l.Register("set_callback", set_callback);
  l.RunScript(
      std::string("function add(a, b)\n"
                  "  return a + b\n"
                  "end\n"
                  "set_callback(add)\n"
                  ""));
  int ret = l.Call<int>(g_ref_1, 10, 20);
  BOOST_CHECK(ret == 30);
}

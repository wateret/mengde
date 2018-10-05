#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "luab/lua.h"

BOOST_AUTO_TEST_CASE(CallLuaFuncFromCpp_1) {
  ::luab::Lua l;
  l.RunScript(
      std::string("function foo()\n"
                  "  return 1\n"
                  "end\n"));
  int ret = l.Call<int>(std::string("foo"));
  BOOST_CHECK(ret == 1);
}

BOOST_AUTO_TEST_CASE(CallLuaFuncFromCpp_2) {
  ::luab::Lua l;
  l.RunScript(
      std::string("function add(a, b)\n"
                  "  return a + b\n"
                  "end\n"));
  int ret = l.Call<int>(std::string("add"), 1, 2);
  BOOST_CHECK(ret == 3);
}

extern "C" {

static ::luab::Ref g_ref_1{-1};

static int set_callback(lua_State* L) {
  ::luab::Lua l{L};
  ::luab::Ref ref = l.Pop<::luab::Ref>();
  g_ref_1 = ref;
  return 0;
}
}  // extern "C"

//
// CalLuaCallbackFromCpp
// 1. Register C function to Lua which registers luab::Ref to g_ref
// 2. Call C function from Lua to add a Lua callback
// 3. Call Lua function with g_ref(a luab::Ref) from C++

BOOST_AUTO_TEST_CASE(CallLuaFunctionWithRefFromCpp_1) {
  ::luab::Lua l;
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

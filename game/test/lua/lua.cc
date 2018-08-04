#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "lua/lua.h"

BOOST_AUTO_TEST_CASE(FunctionCall1) {
  ::lua::Lua l;
  l.RunScript(
      std::string("function foo()\n"
                  "  return 1\n"
                  "end\n"));
  int ret = l.Call<int>(std::string("foo"));
  BOOST_CHECK(ret == 1);
}

BOOST_AUTO_TEST_CASE(FunctionCall2) {
  ::lua::Lua l;
  l.RunScript(
      std::string("function add(a, b)\n"
                  "  return a + b\n"
                  "end\n"));
  int ret = l.Call<int>(std::string("add"), 1, 2);
  BOOST_CHECK(ret == 3);
}

extern "C" {

static ::lua::Ref g_ref{-1};

static int add_callback(lua_State* L) {
  ::lua::Lua l{L};
  ::lua::Ref ref = l.Pop<::lua::Ref>();
  g_ref = ref;
  return 0;
}
}

BOOST_AUTO_TEST_CASE(FunctionCall_Ref1) {
  ::lua::Lua l;
  l.Register("add_callback", add_callback);
  l.RunScript(
      std::string("function add(a, b)\n"
                  "  return a + b\n"
                  "end\n"
                  "add_callback(add)\n"
                  ""));
  int ret = l.Call<int>(g_ref, 10, 20);
  BOOST_CHECK(ret == 30);
}

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

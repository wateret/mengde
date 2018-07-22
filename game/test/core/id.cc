#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include <string>

#include "core/id.h"

using namespace ::mengde::core;

BOOST_AUTO_TEST_CASE(UInt32) {
  Id<uint32_t> id0;
  BOOST_CHECK(id0.IsNone());

  Id<uint32_t> id1{1};
  BOOST_CHECK(id1 == 1);
  BOOST_CHECK(id1 == Id<uint32_t>{1});
  BOOST_CHECK(id1 != 2);
  BOOST_CHECK(id1 != Id<uint32_t>{2});

  id1.SetNone();
  BOOST_CHECK(id1.IsNone());

  id1 = 1;
  BOOST_CHECK(id1 == 1);
  BOOST_CHECK(id1.Value() == 1);

  id1 = id0;
  BOOST_CHECK(id1.IsNone());
}

BOOST_AUTO_TEST_CASE(String1) {
  Id<std::string> id0;
  BOOST_CHECK(id0.IsNone());

  Id<std::string> id1{std::string("id1")};
  BOOST_CHECK(id1 == std::string("id1"));
}

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include <string>

#include "core/id.h"

using namespace ::mengde::core;

BOOST_AUTO_TEST_CASE(uint32_t_Constructors) {
  Id<uint32_t> id0;
  BOOST_CHECK(id0.IsNone());
  BOOST_CHECK(!id0);

  Id<uint32_t> id1{1u};
  BOOST_CHECK(id1 == 1u);
  BOOST_CHECK(id1 == Id<uint32_t>{1u});
  BOOST_CHECK(id1 != 100u);
  BOOST_CHECK(id1 != Id<uint32_t>{100u});
  BOOST_CHECK(id1);

  uint32_t id2_val = 2u;
  Id<uint32_t> id2{id2_val};
  BOOST_CHECK(id2 == id2_val);
  BOOST_CHECK(id2 != id1);

  Id<uint32_t> id3{Id<uint32_t>{3u}};
  BOOST_CHECK(id3 == 3u);

  Id<uint32_t> id4{id3};
  BOOST_CHECK(id4 == id3);
}

BOOST_AUTO_TEST_CASE(uint32_t_Assignments) {
  Id<uint32_t> id0;

  id0 = 0u;
  BOOST_CHECK(id0 == 0u);

  uint32_t val1 = 1u;
  id0 = val1;
  BOOST_CHECK(id0 == 1u);

  id0 = Id<uint32_t>{2u};
  BOOST_CHECK(id0 == Id<uint32_t>{2u});
  BOOST_CHECK(id0 == 2u);

  Id<uint32_t> idt;
  id0 = idt;
  BOOST_CHECK(id0 == idt);
}

BOOST_AUTO_TEST_CASE(std_string_General) {
  Id<std::string> id0;
  BOOST_CHECK(id0.IsNone());

  Id<std::string> id1{std::string("id1")};
  BOOST_CHECK(id1 == std::string("id1"));
}

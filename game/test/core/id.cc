#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include <string>

#include "core/id.h"

using namespace ::mengde::core;

using UInt32Id = Id<uint32_t, struct Tag_UInt32>;

BOOST_AUTO_TEST_CASE(uint32_t_Constructors) {
  UInt32Id id0;
  BOOST_CHECK(id0.IsNone());
  BOOST_CHECK(!id0);

  UInt32Id id1{1u};
  BOOST_CHECK(id1 == 1u);
  BOOST_CHECK(id1 == UInt32Id{1u});
  BOOST_CHECK(id1 != 100u);
  BOOST_CHECK(id1 != UInt32Id{100u});
  BOOST_CHECK(id1);

  uint32_t id2_val = 2u;
  UInt32Id id2{id2_val};
  BOOST_CHECK(id2 == id2_val);
  BOOST_CHECK(id2 != id1);

  UInt32Id id3{UInt32Id{3u}};
  BOOST_CHECK(id3 == 3u);

  UInt32Id id4{id3};
  BOOST_CHECK(id4 == id3);
}

BOOST_AUTO_TEST_CASE(uint32_t_Assignments) {
  UInt32Id id0;

  id0 = 0u;
  BOOST_CHECK(id0 == 0u);

  uint32_t val1 = 1u;
  id0 = val1;
  BOOST_CHECK(id0 == 1u);

  id0 = UInt32Id{2u};
  BOOST_CHECK(id0 == UInt32Id{2u});
  BOOST_CHECK(id0 == 2u);

  UInt32Id idt;
  id0 = idt;
  BOOST_CHECK(id0 == idt);
}

using StringId = Id<std::string, struct Tag_String>;

BOOST_AUTO_TEST_CASE(std_string_General) {
  StringId id0;
  BOOST_CHECK(id0.IsNone());

  StringId id1{std::string("id1")};
  BOOST_CHECK(id1 == std::string("id1"));
}

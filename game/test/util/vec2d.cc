#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "util/vec2d.h"

BOOST_AUTO_TEST_CASE(Test1) {
  Vec2D vec{1, 2};
  Vec2D exp{2, 4};
  BOOST_CHECK(vec + vec == exp);
}

BOOST_AUTO_TEST_CASE(Test2) {
  Vec2D vec{1, 2};
  Vec2D exp{2, 4};
  BOOST_CHECK(vec * 2 == exp);
}

BOOST_AUTO_TEST_CASE(Test3) {
  Vec2D vec1{1, 2};
  Vec2D vec2{3, 4};
  Vec2D exp{4, 6};
  BOOST_CHECK(vec1 + vec2 == exp);
}

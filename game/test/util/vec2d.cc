#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "util/vec2d.h"

BOOST_AUTO_TEST_CASE(Test1) {
  Vec2D vec{1, 2};
  Vec2D exp{2, 4};
  BOOST_CHECK(vec + vec == exp);
}

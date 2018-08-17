#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "gui/foundation/rect.h"

using namespace mengde::gui::foundation;

BOOST_AUTO_TEST_CASE(Rect_Basics) {
  Rect r{100, 200, 300, 400};

  BOOST_CHECK(r.x() == 100);
  BOOST_CHECK(r.y() == 200);
  BOOST_CHECK(r.w() == 300);
  BOOST_CHECK(r.h() == 400);

  BOOST_CHECK(r.left() == 100);
  BOOST_CHECK(r.top() == 200);
  BOOST_CHECK(r.right() == 399);
  BOOST_CHECK(r.bottom() == 599);

  r += 100;

  BOOST_CHECK(r.x() == 100);
  BOOST_CHECK(r.y() == 200);
  BOOST_CHECK(r.w() == 400);
  BOOST_CHECK(r.h() == 500);

  r -= 100;

  BOOST_CHECK(r.x() == 100);
  BOOST_CHECK(r.y() == 200);
  BOOST_CHECK(r.w() == 300);
  BOOST_CHECK(r.h() == 400);
}

BOOST_AUTO_TEST_CASE(Rect_Contains) {
  Rect r{100, 100, 100, 100};

  // Vec2D Contains

  BOOST_CHECK(!r.Contains(Vec2D{50, 50}));
  BOOST_CHECK(r.Contains(Vec2D{100, 100}));
  BOOST_CHECK(r.Contains(Vec2D{150, 150}));
  BOOST_CHECK(r.Contains(Vec2D{199, 199}));
  BOOST_CHECK(!r.Contains(Vec2D{200, 200}));

  // Rect Contains

  Rect r2{100, 100, 100, 100};
  BOOST_CHECK(r.Contains(r2));
  BOOST_CHECK(r2.Contains(r));

  Rect r3{125, 125, 50, 50};
  BOOST_CHECK(r.Contains(r3));
  BOOST_CHECK(!r3.Contains(r));

  Rect r4{150, 150, 100, 100};
  BOOST_CHECK(!r.Contains(r4));
  BOOST_CHECK(!r4.Contains(r));
}

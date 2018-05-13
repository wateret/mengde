#ifndef UTIL_VEC2D_H_
#define UTIL_VEC2D_H_

struct Vec2D {
  int x, y;
  Vec2D(int x, int y) : x(x), y(y) {}
  Vec2D() : Vec2D(0, 0) {}
  Vec2D Bound(const Vec2D& lo, const Vec2D& up) const;
  Vec2D operator+(const int d) const { return Vec2D(x + d, y + d); }
  Vec2D operator+(const Vec2D& o) const { return Vec2D(x + o.x, y + o.y); }
  Vec2D operator-(const int d) const { return Vec2D(x - d, y - d); }
  Vec2D operator-(const Vec2D& o) const { return Vec2D(x - o.x, y - o.y); }
  Vec2D operator*(const int m) const { return Vec2D(x * m, y * m); }
  Vec2D operator*(const float m) const { return Vec2D(x * m, y * m); }
  Vec2D operator*(const Vec2D& o) const { return Vec2D(x * o.x, y * o.y); }
  Vec2D operator/(const int m) const { return Vec2D(x / m, y / m); }
  Vec2D operator/(const Vec2D& o) const { return Vec2D(x / o.x, y / o.y); }
  Vec2D operator%(const int m) const { return Vec2D(x % m, y % m); }
  Vec2D operator%(const Vec2D& o) const { return Vec2D(x % o.x, y % o.y); }
  Vec2D operator-(void) const { return Vec2D(-x, -y); }
  Vec2D& operator+=(const Vec2D o) {
    x += o.x;
    y += o.y;
    return *this;
  }
  Vec2D& operator-=(const Vec2D o) {
    x -= o.x;
    y -= o.y;
    return *this;
  }
  Vec2D& operator*=(const Vec2D o) {
    x *= o.x;
    y *= o.y;
    return *this;
  }
  Vec2D& operator/=(const Vec2D o) {
    x /= o.x;
    y /= o.y;
    return *this;
  }
  bool operator==(const Vec2D o) const { return x == o.x && y == o.y; }
  bool operator!=(const Vec2D o) const { return x != o.x || y != o.y; }
  bool operator<(const Vec2D o) const { return x < o.x && y < o.y; }
  bool operator<=(const Vec2D o) const { return x <= o.x && y <= o.y; }
  bool operator>(const Vec2D o) const { return x > o.x && y > o.y; }
  bool operator>=(const Vec2D o) const { return x >= o.x && y >= o.y; }
};

#endif  // UTIL_Vec2D_H_

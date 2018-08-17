#ifndef MENGDE_GUI_FOUNDATION_RECT_H_
#define MENGDE_GUI_FOUNDATION_RECT_H_

#include <SDL.h>
#include "util/common.h"

namespace mengde {
namespace gui {
namespace foundation {

class Rect {
 public:
  Rect(int, int, int, int);
  Rect(Vec2D, Vec2D);
  Rect();

 public:
  const SDL_Rect* GetRawRectPtr() const;

  // Getters
 public:
  int x() const { return rect_.x; }
  int y() const { return rect_.y; }
  int w() const { return rect_.w; }
  int h() const { return rect_.h; }
  Vec2D size() const { return Vec2D(rect_.w, rect_.h); }
  Vec2D pos() const { return Vec2D(rect_.x, rect_.y); }
  int left() const { return x(); }
  int top() const { return y(); }
  int right() const { return rect_.x + rect_.w - 1; }
  int bottom() const { return rect_.y + rect_.h - 1; }

  // Setters
 public:
  void x(int v) { rect_.x = v; }
  void y(int v) { rect_.y = v; }
  void w(int v) { rect_.w = v; }
  void h(int v) { rect_.h = v; }
  void pos(Vec2D);
  void size(Vec2D);

  // Other modifiers
 public:
  void Contract(int);
  void Move(int, int);
  void Move(Vec2D);
  Rect& Magnify(int);
  Rect& Magnify(Vec2D);

  bool Contains(Vec2D) const;
  bool Contains(const Rect& rect) const;

 public:
  Rect operator*(int)const;
  Rect& operator*=(int);
  Rect& operator*=(Vec2D);
  Rect& operator+=(int);
  Rect& operator+=(Vec2D);
  Rect& operator-=(int);
  Rect& operator-=(Vec2D);

 private:
  SDL_Rect rect_;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_FOUNDATION_RECT_H_

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
  int             GetX() const { return rect_.x; }
  int             GetY() const { return rect_.y; }
  int             GetW() const { return rect_.w; }
  int             GetH() const { return rect_.h; }
  int             GetRight() const { return rect_.x + rect_.w; }
  int             GetBottom() const { return rect_.y + rect_.h; }
  void            SetX(int v) { rect_.x = v; }
  void            SetY(int v) { rect_.y = v; }
  void            SetW(int v) { rect_.w = v; }
  void            SetH(int v) { rect_.h = v; }
  Vec2D           GetSize() const { return Vec2D(rect_.w, rect_.h); }
  Vec2D           GetPos() const { return Vec2D(rect_.x, rect_.y); }
  void            Contract(int);
  void            Move(int, int);
  void            Move(Vec2D);
  void            SetPos(Vec2D);
  void            SetSize(Vec2D);
  bool            Contains(Vec2D) const;
  Rect            operator*(int)const;
  Rect&           operator*=(int);
  Rect&           operator*=(Vec2D);
  Rect&           operator+=(int);
  Rect&           operator+=(Vec2D);
  Rect&           operator-=(int);
  Rect&           operator-=(Vec2D);
  Rect&           Magnify(int);
  Rect&           Magnify(Vec2D);

 private:
  SDL_Rect rect_;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_FOUNDATION_RECT_H_

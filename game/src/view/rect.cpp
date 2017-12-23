#include "rect.h"

Rect::Rect(int x, int y, int w, int h) {
  rect_.x = x;
  rect_.y = y;
  rect_.w = w;
  rect_.h = h;
}

Rect::Rect(Vec2D pos, Vec2D size) {
  rect_.x = pos.x;
  rect_.y = pos.y;
  rect_.w = size.x;
  rect_.h = size.y;
}

Rect::Rect() : Rect(0, 0, 0, 0) {
}

Rect Rect::operator*(int m) const {
  return Rect(rect_.x, rect_.y, rect_.w * m, rect_.h * m);
}

Rect& Rect::operator*=(int m) {
  rect_.w *= m;
  rect_.h *= m;
  return *this;
}

Rect& Rect::operator*=(Vec2D v) {
  rect_.w *= v.x;
  rect_.h *= v.y;
  return *this;
}

Rect& Rect::operator+=(int m) {
  rect_.w += m;
  rect_.h += m;
  return *this;
}

Rect& Rect::operator+=(Vec2D v) {
  rect_.w += v.x;
  rect_.h += v.y;
  return *this;
}

Rect& Rect::operator-=(int m) {
  rect_.w -= m;
  rect_.h -= m;
  return *this;
}

Rect& Rect::operator-=(Vec2D v) {
  rect_.w -= v.x;
  rect_.h -= v.y;
  return *this;
}

Rect& Rect::Magnify(int m) {
  rect_.x *= m;
  rect_.y *= m;
  rect_.w *= m;
  rect_.h *= m;
  return *this;
}

Rect& Rect::Magnify(Vec2D v) {
  rect_.x *= v.x;
  rect_.y *= v.y;
  rect_.w *= v.x;
  rect_.h *= v.y;
  return *this;
}

const SDL_Rect* Rect::GetRawRectPtr() const {
  return &rect_;
}

void Rect::Move(int dx, int dy) {
  rect_.x += dx;
  rect_.y += dy;
}

void Rect::Move(Vec2D v) {
  Move(v.x, v.y);
}

void Rect::SetPos(Vec2D v) {
  rect_.x = v.x;
  rect_.y = v.y;
}

void Rect::SetSize(Vec2D v) {
  rect_.w = v.x;
  rect_.h = v.y;
}

void Rect::Contract(int amount) {
  rect_.x += amount;
  rect_.y += amount;
  rect_.w -= amount * 2;
  rect_.h -= amount * 2;
}

bool Rect::Contains(Vec2D v) const {
  return rect_.x <= v.x && v.x <= rect_.x + rect_.w &&
         rect_.y <= v.y && v.y <= rect_.y + rect_.h;
}

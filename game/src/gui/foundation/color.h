#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>
#include <string>
#include <unordered_map>

typedef unsigned char uint8_t;

struct Color {
  uint8_t r, g, b, a;

  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    : r(r), g(g), b(b), a(a) {
  }

  SDL_Color ToRawColor() { return *(SDL_Color*)(this); }
  std::string ToHexString() {
    char buf[9];
    static const char table[] = "0123456789abcdef";
    buf[0] = table[r & 0xf];
    buf[1] = table[(r >> 4) & 0xf];
    buf[2] = table[g & 0xf];
    buf[3] = table[(g >> 4) & 0xf];
    buf[4] = table[b & 0xf];
    buf[5] = table[(b >> 4) & 0xf];
    buf[6] = table[a & 0xf];
    buf[7] = table[(a >> 4) & 0xf];
    buf[8] = '\0';
    return std::string(buf);
  }
};

class ColorGen {
 public:
  static ColorGen* GetInstance();
  Color Generate(const std::string& /* name */);
  Color Generate(const std::string& /* name */, uint8_t /* alpha */);
  Color Generate(uint8_t /* r */, uint8_t /* r */, uint8_t /* r */, uint8_t /* alpha */ = 255);

 private:
  ColorGen();

 private:
  std::unordered_map<std::string, Color> colors_;
};

#define COLOR ColorGen::GetInstance()->Generate
#define COLOR ColorGen::GetInstance()->Generate

#endif

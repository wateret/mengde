#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>
#include <string>

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

#define COLOR_TRANSPARENT {255, 255, 255, 0}
#define COLOR_WHITE       {255, 255, 255, 255}
#define COLOR_BLACK       {0, 0, 0, 255}
#define COLOR_RED         {255, 0, 0, 255}
#define COLOR_BLUE        {0, 0, 255, 255}
#define COLOR_GREEN       {0, 255, 0, 255}
#define COLOR_GRAY        {128, 128, 128, 255}
#define COLOR_DARKGRAY    {64, 64, 64, 255}
#define COLOR_YELLOW      {255, 255, 0, 255}
#define COLOR_LIGHTYELLOW {255, 255, 224, 255}
#define COLOR_NAVY        {0, 0, 128, 255}
#define COLOR_ORANGE      {255, 165, 0, 255}

#define COLORC_GAUGE_STATS {255, 203, 107, 255}
#define COLORC_GAUGE_BG    COLOR_GRAY
#define COLORC_GAUGE_HP    {64, 192, 64, 255}
#define COLORC_GAUGE_MP    {64, 192, 224, 255}
#define COLORC_GAUGE_EXP   {199, 146, 234, 255}
#define COLORC_GAUGE_HPRED {192, 64, 64, 255}

#endif

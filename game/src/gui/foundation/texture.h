#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "window.h"
#include "rect.h"
#include "color.h"
#include "util/common.h"

class Renderer;

class Texture {
 public:
  Texture(Renderer*, const std::string&);             // bitmap
  Texture(Renderer*, const std::string&, Color);      // bitmap with colorkey
  Texture(Renderer*, const std::string&, TTF_Font*, Color, uint32_t = 0, uint32_t = 0); // string
  Texture(Renderer*, SDL_Surface*);
  Texture(SDL_Texture*, int, int);
  ~Texture();

 public:
  SDL_Texture* AsRawTexture();
  int GetW();
  int GetH();
  Vec2D GetSize();
  void SetAlpha(uint8_t);
  void SetShade(uint8_t);
  bool Loaded() { return texture_ != nullptr; }
  Texture* NewWhitenedTexture(Renderer*);

 private:
  void InitBitmap(Renderer*, const std::string&, bool, Color);
  void CreateFromSurface(Renderer*, SDL_Surface*);
  void FreeIfLoaded();

 private:
  SDL_Surface* surface_; // We hold surface for manipulating sprites
  SDL_Texture* texture_;
  int width_;
  int height_;
};

#endif // TEXTURE_H

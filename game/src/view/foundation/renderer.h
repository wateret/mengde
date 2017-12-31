#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include "window.h"
#include "rect.h"
#include "color.h"

class Texture;

class Renderer {
 public:
  Renderer(Window*);
  ~Renderer();

 public:
  void CopyTexture(Texture*, Rect* = NULL, Rect* = NULL, bool = false);
  void DrawRect(const Rect*, const int = 1);
  void FillRect(const Rect*);
  void SetDrawColor(Color);
  void Present();
  void Clear();
  void SetViewport(const Rect*);
  SDL_Renderer* GetRawRenderer();
  uint32_t GetPixelFormat();
  SDL_PixelFormat* GetRawPixelFormatObj();

 private:
  SDL_Renderer* handle_;
  uint32_t pixel_format_;
  SDL_PixelFormat* pixel_format_obj_;
//  Window* window_;    // renderer is tied to a window
};

#endif // RENDERER_H

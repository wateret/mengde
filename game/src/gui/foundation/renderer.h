#ifndef MENGDE_GUI_FOUNDATION_RENDERER_H_
#define MENGDE_GUI_FOUNDATION_RENDERER_H_

#include <SDL.h>
#include "color.h"
#include "rect.h"
#include "window.h"

namespace mengde {
namespace gui {
namespace foundation {

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

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_FOUNDATION_RENDERER_H_

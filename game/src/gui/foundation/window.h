#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <string>
#include "surface.h"
#include "util/common.h"

namespace mengde {
namespace gui {
namespace foundation {

class Window {
 public:
  Window(const std::string&, int, int);
  ~Window();
  void        Update();
  void        InfoMessageBox(const char* title, const char* message);
  SDL_Window* AsRawWindow();
  Vec2D       GetSize();

 private:
  SDL_Window* handle_;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif
